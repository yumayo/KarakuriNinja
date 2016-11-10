# include "EnemySlash.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    EnemySlash::EnemySlash( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, "EnemySlash.png" )
        , timer( )
        , isAttack( false )
        , prevMovePosition( Vec3f::zero( ) )
    {
        SetFunction( &EnemySlash::タイマーが鳴るまで待機 );
    }
    void EnemySlash::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // 重力処理
            Gravitate( );

            // メイン処理
            behavior( camera );

            // タイマー処理
            timer.Update( );
        }

        EnemyBase::update( camera );
    }
    void EnemySlash::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemySlash::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack;
    }
    void EnemySlash::Gravitate( )
    {
        //　ジャンプ中なら重力をかけます。
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemySlash::タイマーが鳴るまで待機( cinder::CameraPersp const& camera )
    {
        // タイマーが鳴ったら次の関数へ。
        if ( timer.IsAction( ) )
        {
            prevMovePosition = object.Position( );
            timer.Advance( 120 );
            SetFunction( &EnemySlash::カメラへ近づく );
            return;
        }
    }
    void EnemySlash::カメラへ近づく( cinder::CameraPersp const& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );

        auto x = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.x, cameraPos.x );
        auto z = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.z, cameraPos.z );

        object.Position( Vec3f( x, object.Position( ).y, z ) );

        if ( timer.IsAction( ) )
        {
            timer.Advance( 60 ); // 攻撃モーションフレームを代入
            SetFunction( &EnemySlash::攻撃モーション );
            return;
        }
    }
    void EnemySlash::攻撃モーション( cinder::CameraPersp const& camera )
    {
        // モーション時間60フレームの後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemySlash::攻撃 );
            return;
        }
    }
    void EnemySlash::攻撃( cinder::CameraPersp const& camera )
    {
        // 攻撃してすぐ次の関数へ。
        isAttack = true;
        timer.Advance( 10 ); // 硬直10フレーム
        SetFunction( &EnemySlash::攻撃後硬直 );
        return;
    }
    void EnemySlash::攻撃後硬直( cinder::CameraPersp const& camera )
    {
        // 攻撃フラグをオフに、硬直時間10フレームの後、次の関数へ。
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemySlash::ジャンプで戻る );
            return;
        }
    }
    void EnemySlash::ジャンプで戻る( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 8.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 4.0F, M_PI / 4.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // 硬直後、すぐにジャンプして次の関数へ。
        Jump( jumpPower + backDirection );

        SetFunction( &EnemySlash::着地 );
        return;
    }
    void EnemySlash::着地( cinder::CameraPersp const& camera )
    {
        // 地面に着いたら、次の関数へ。
        if ( isLanding )
        {
            // スピードは0にします。滑っちゃうので。
            object.Speed( Vec3f::zero( ) );
            // 次の攻撃までのフレームを代入。
            timer.Advance( randInt( 60, 240 ) );
            SetFunction( &EnemySlash::タイマーが鳴るまで待機 );
            return;
        }
    }

    bool EnemySlash::IsJumping( )
    {
        return isLanding == false;
    }
    void EnemySlash::SetFunction( void( EnemySlash::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}