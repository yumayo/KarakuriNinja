# include "EnemyTutorial.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

# include "GlobalData.hpp"

# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    EnemyTutorial::EnemyTutorial( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 6.5F, 3 ) )
        , timer( )
        , isAttack( false )
        , prevMovePosition( Vec3f::zero( ) )
    {
        TRData::Reset( );

        int index = 1;
        待機 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        攻撃モーション画像 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        攻撃画像 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        左に移動 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        右に移動 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        texture = 待機;
        knockBackTexture = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemyTutorial::タイマーが鳴るまで待機 );
    }
    EnemyTutorial::~EnemyTutorial( )
    {
        if ( !TRData::enemyKill.IsComplete( ) )
        {
            TRData::enemyKill.TutorialStart( );
        }
    }
    void EnemyTutorial::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // 重力処理
            Gravitate( );

            // ノックバック時は一時的に処理をしません。
            if ( !IsKnockBack( ) )
            {
                // メイン処理
                behavior( camera );

                // タイマー処理
                timer.Update( );
            }
        }
        else
        {
            texture = knockBackTexture;
        }

        EnemyBase::update( camera );

        // EnemyBase::updateよりもあとの処理である必要があります。
        // タイマー系の更新が必要なため。
        if ( IsAttackMotion( ) )
        {
            if ( !TRData::attackCircle.IsComplete( ) )
            {
                TRData::attackCircle.TutorialStart( );
            }
        }

        if ( IsAttackOneFramePrev( ) )
        {
            if ( !TRData::guard.IsComplete( ) )
            {
                TRData::guard.TutorialStart( );
            }
        }
    }
    void EnemyTutorial::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemyTutorial::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && IsLive( );
    }
    void EnemyTutorial::Gravitate( )
    {
        //　ジャンプ中なら重力をかけます。
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemyTutorial::何もしない( cinder::CameraPersp const & camera )
    {

    }
    void EnemyTutorial::タイマーが鳴るまで待機( cinder::CameraPersp const& camera )
    {
        // エネミーが出現した時にチュートリアルを呼ぶ
        if ( !TRData::spawn.IsComplete( ) )
        {
            TRData::spawn.TutorialStart( );
        }

        prevMovePosition = object.Position( );
        timer.Advance( 120 );
        SetFunction( &EnemyTutorial::カメラへ近づく );
        return;
    }
    void EnemyTutorial::カメラへ近づく( cinder::CameraPersp const& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );

        auto x = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.x, cameraPos.x );
        auto z = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.z, cameraPos.z );

        object.Position( Vec3f( x, object.Position( ).y, z ) );

        if ( timer.IsAction( ) )
        {
            timer.Advance( 60 ); // 攻撃モーションフレームを代入
            attackTime.AttackFrame( 60 );
            texture = 攻撃モーション画像;
            SetFunction( &EnemyTutorial::攻撃モーション );
            return;
        }
    }
    void EnemyTutorial::攻撃モーション( cinder::CameraPersp const& camera )
    {
        // モーション時間60フレームの後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            texture = 攻撃画像;
            SetFunction( &EnemyTutorial::攻撃 );
            return;
        }
    }
    void EnemyTutorial::攻撃( cinder::CameraPersp const& camera )
    {
        // 攻撃してすぐ次の関数へ。
        isAttack = true;
        timer.Advance( 10 ); // 硬直10フレーム
        SetFunction( &EnemyTutorial::攻撃後硬直 );
        return;
    }
    void EnemyTutorial::攻撃後硬直( cinder::CameraPersp const& camera )
    {
        // 攻撃フラグをオフに、硬直時間10フレームの後、次の関数へ。
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            texture = 待機;
            SetFunction( &EnemyTutorial::ジャンプで戻る );
            return;
        }
    }
    void EnemyTutorial::ジャンプで戻る( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 8.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 8.0F, M_PI / 8.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // 硬直後、すぐにジャンプして次の関数へ。
        Jump( jumpPower + backDirection );

        SetFunction( &EnemyTutorial::着地 );
        return;
    }
    void EnemyTutorial::着地( cinder::CameraPersp const& camera )
    {
        // 地面に着いたら、次の関数へ。
        if ( isLanding )
        {
            // 攻撃のチュートリアルを行います。
            if ( !TRData::playerAttack.IsComplete( ) )
            {
                TRData::playerAttack.TutorialStart( );
            }

            // スピードは0にします。滑っちゃうので。
            object.Speed( Vec3f::zero( ) );
            // 次の攻撃までのフレームを代入。
            timer.Advance( randInt( 60, 240 ) );
            SetFunction( &EnemyTutorial::何もしない );
            return;
        }
    }

    bool EnemyTutorial::IsJumping( )
    {
        return isLanding == false;
    }
    void EnemyTutorial::SetFunction( void( EnemyTutorial::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}