# include "EnemyBullet.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

namespace User
{
    using namespace cinder;
    EnemyBullet::EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, "EnemyBullet.png" )
    {
        SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
    }
    void EnemyBullet::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // メイン処理
            behavior( camera );

            // タイマー更新
            timer.Update( );
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
            SetFunction( &EnemyBullet::弾を投げる );
        }
    }
    void EnemyBullet::弾を投げる( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( 0 + 50, env.getWindowWidth( ) - 50 ) / env.getWindowWidth( );
        auto v = randFloat( 0 + 50, env.getWindowHeight( ) - 50 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken2.png" ) );

        // タイマーをセットしてまた待機状態にします。
        timer.Advance( randInt( 120, 300 ) );
        SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
    }
    void EnemyBullet::SetFunction( void( EnemyBullet::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}