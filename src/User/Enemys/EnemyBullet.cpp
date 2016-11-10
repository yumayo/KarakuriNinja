# include "EnemyBullet.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;
    EnemyBullet::EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 6.0F, 4 ) )
    {
        timer.Advance( 120 );

        int index = 1;
        待機 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        攻撃モーション画像 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        攻撃画像 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        左に移動 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        右に移動 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );

        texture = 待機;
        knockBackTexture = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
        }
    }
    void EnemyBullet::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
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
    void EnemyBullet::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemyBullet::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    void EnemyBullet::タイマーが鳴るまで待機( cinder::CameraPersp const & camera )
    {
        // 行動時間になったら弾を投げます。
        if ( timer.IsAction( ) )
        {
            if ( randInt( 0, 3 ) != 0 )
            {
                texture = 攻撃モーション画像;
                timer.Advance( 60 );
                SetFunction( &EnemyBullet::攻撃モーション );
            }
            else
            {
                timer.Advance( randInt( 30, 60 ) );
                if ( randBool( ) )
                {
                    texture = 左に移動;
                    SetFunction( &EnemyBullet::左へ移動 );
                }
                else
                {
                    texture = 右に移動;
                    SetFunction( &EnemyBullet::右へ移動 );
                }
            }
        }
    }
    void EnemyBullet::攻撃モーション( cinder::CameraPersp const & camera )
    {
        // モーション時間60フレームの後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            texture = 攻撃画像;
            SetFunction( &EnemyBullet::攻撃 );
            return;
        }
    }
    void EnemyBullet::攻撃( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( env.getWindowWidth( )*0.25, env.getWindowWidth( )*0.75 ) / env.getWindowWidth( );
        auto v = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png" ) );
        EffectCreate( EffectBase( "Textures/Effect/effect4.png",
                                  camera.worldToScreen( object.Position( ), env.getWindowWidth( ), env.getWindowHeight( ) ),
                                  camera.worldToEyeDepth( object.Position( ) ),
                                  Vec2f( 240, 240 ),
                                  Vec2f( 240, 240 ),
                                  EffectBase::Mode::CENTERCENTER
        ) );

        // タイマーをセットしてまた待機状態にします。
        timer.Advance( 10 );
        SetFunction( &EnemyBullet::攻撃後硬直 );
        se[randInt( 3 )]->Play( );


    }
    void EnemyBullet::攻撃後硬直( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 120, 300 ) );
            texture = 待機;
            SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBullet::左へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 30, 60 ) );
            texture = 右に移動;
            SetFunction( &EnemyBullet::右へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
        }
    }
    void EnemyBullet::右へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( -M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            timer.Advance( randInt( 20, 30 ) );
            texture = 左に移動;
            SetFunction( &EnemyBullet::左へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 120, 200 ) );
            texture = 待機;
            SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
        }
    }
    void EnemyBullet::SetFunction( void( EnemyBullet::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}