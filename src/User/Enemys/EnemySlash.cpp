# include "EnemySlash.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EnemySlash::EnemySlash( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 8.0F, 3 ) )
        , timer( )
        , isAttack( false )
        , prevMovePosition( Vec3f::zero( ) )
    {
        timer.Advance( randInt( 200, 300 ) );

        int index = 1;
        待機 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        攻撃モーション画像 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        攻撃画像 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        左に移動 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        右に移動 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        texture = 待機;
        knockBackTexture = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemySlash::タイマーが鳴るまで待機 );
    }
    void EnemySlash::update( cinder::CameraPersp const& camera )
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
    }
    void EnemySlash::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemySlash::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && IsLive( ) && !IsKnockBack( );
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
            if ( randInt( 0, 3 ) != 0 )
            {
                prevMovePosition = object.Position( );

                auto cameraRightVec = camera.getViewDirection( );
                cameraRightVec.rotate( camera.getWorldUp( ), -M_PI / 2.0 );
                targetVector = cameraRightVec * randFloat( -1, 1 );

                timer.Advance( 120 );
                SetFunction( &EnemySlash::カメラへ近づく );
                return;
            }
            else
            {
                timer.Advance( randInt( 30, 60 ) );
                if ( randBool( ) )
                {
                    texture = 左に移動;
                    SetFunction( &EnemySlash::左へ移動 );
                }
                else
                {
                    texture = 右に移動;
                    SetFunction( &EnemySlash::右へ移動 );
                }
            }
        }
    }
    void EnemySlash::カメラへ近づく( cinder::CameraPersp const& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );

        auto targetPos = cameraPos + targetVector;

        auto x = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.x, targetPos.x );
        auto z = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.z, targetPos.z );

        object.Position( Vec3f( x, object.Position( ).y, z ) );

        if ( timer.IsAction( ) )
        {
            timer.Advance( 60 ); // 攻撃モーションフレームを代入
            attackTime.AttackFrame( 60 );
            prevAttackHP = status.HP;
            texture = 攻撃モーション画像;
            SetFunction( &EnemySlash::攻撃モーション );
            return;
        }
    }
    void EnemySlash::攻撃モーション( cinder::CameraPersp const& camera )
    {
        // モーション時間60フレームの後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            texture = 攻撃画像;
            SetFunction( &EnemySlash::攻撃 );
            return;
        }

        if ( prevAttackHP != status.HP )
        {
            texture = 待機;
            timer.Advance( 10 ); // 硬直10フレーム
            SetFunction( &EnemySlash::攻撃後硬直 );
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
            texture = 待機;
            SetFunction( &EnemySlash::ジャンプで戻る );
            return;
        }
    }
    void EnemySlash::ジャンプで戻る( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * randFloat( 6.0F, 12.0F ) / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 8.0F, M_PI / 8.0F ) );
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

    void EnemySlash::左へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 20, 30 ) );
            texture = 右に移動;
            SetFunction( &EnemySlash::右へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemySlash::タイマーが鳴るまで待機 );
        }
    }
    void EnemySlash::右へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( -M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 20, 30 ) );
            texture = 左に移動;
            SetFunction( &EnemySlash::左へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemySlash::タイマーが鳴るまで待機 );
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