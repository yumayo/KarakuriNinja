# include "EnemyBulletTutorial.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;
    EnemyBulletTutorial::EnemyBulletTutorial( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 80.0F, 0 ) )
    {
        timer.Advance( 60 );

        int index = 1;
        待機 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        攻撃モーション画像 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        攻撃画像 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        index++;
        index++;

        texture = 待機;
        knockBackTexture = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemyBulletTutorial::タイマーが鳴るまで待機 );
        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
        }
    }
    void EnemyBulletTutorial::update( cinder::CameraPersp const& camera )
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
    void EnemyBulletTutorial::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemyBulletTutorial::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    void EnemyBulletTutorial::タイマーが鳴るまで待機( cinder::CameraPersp const & camera )
    {
        // 行動時間になったら弾を投げます。
        if ( timer.IsAction( ) )
        {
            texture = 攻撃モーション画像;
            timer.Advance( 60 );
            SetFunction( &EnemyBulletTutorial::攻撃モーション );
        }
    }
    void EnemyBulletTutorial::攻撃モーション( cinder::CameraPersp const & camera )
    {
        // モーション時間60フレームの後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            texture = 攻撃画像;
            SetFunction( &EnemyBulletTutorial::攻撃 );
            return;
        }
    }
    void EnemyBulletTutorial::攻撃( cinder::CameraPersp const & camera )
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
        SetFunction( &EnemyBulletTutorial::攻撃後硬直 );
        se[randInt( 3 )]->Play( );
    }
    void EnemyBulletTutorial::攻撃後硬直( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( 90 );
            texture = 待機;
            SetFunction( &EnemyBulletTutorial::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBulletTutorial::SetFunction( void( EnemyBulletTutorial::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}