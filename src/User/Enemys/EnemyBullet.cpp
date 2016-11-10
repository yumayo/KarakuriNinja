# include "EnemyBullet.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

namespace User
{
    using namespace cinder;
    EnemyBullet::EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& fieldName )
        : EnemyBase( pos, camera )
        , 待機( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (1).png" ) ) ) )
        , 攻撃モーション画像( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (2).png" ) ) ) )
        , 攻撃画像( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (3).png" ) ) ) )
        , 左に移動( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (4).png" ) ) ) )
        , 右に移動( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (5).png" ) ) ) )
    {
        textureRef = 待機;

        SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
        for ( int i = 0; i < 3; i++ ) {
            se.push_back( Audio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
        }
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
            if ( randInt( 0, 3 ) != 0 )
            {
                textureRef = 攻撃モーション画像;
                timer.Advance( 60 );
                SetFunction( &EnemyBullet::攻撃モーション );
            }
            else
            {
                timer.Advance( randInt( 30, 60 ) );
                if ( randBool( ) )
                {
                    textureRef = 左に移動;
                    SetFunction( &EnemyBullet::左へ移動 );
                }
                else
                {
                    textureRef = 右に移動;
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
            textureRef = 攻撃画像;
            SetFunction( &EnemyBullet::攻撃 );
            return;
        }
    }
    void EnemyBullet::攻撃( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( 0 + 100, env.getWindowWidth( ) - 100 ) / env.getWindowWidth( );
        auto v = randFloat( 0 + 100, env.getWindowHeight( ) - 300 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken2.png" ) );

        // タイマーをセットしてまた待機状態にします。
        timer.Advance( 10 );
        SetFunction( &EnemyBullet::攻撃後硬直 );
        se[randInt( 3 )].Play( );
    }
    void EnemyBullet::攻撃後硬直( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 120, 300 ) );
            textureRef = 待機;
            SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBullet::左へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInTheScreen( camera ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 20, 30 ) );
            textureRef = 右に移動;
            SetFunction( &EnemyBullet::右へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            textureRef = 待機;
            SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
        }
    }
    void EnemyBullet::右へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( -M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInTheScreen( camera ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 20, 30 ) );
            textureRef = 左に移動;
            SetFunction( &EnemyBullet::左へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            textureRef = 待機;
            SetFunction( &EnemyBullet::タイマーが鳴るまで待機 );
        }
    }
    void EnemyBullet::SetFunction( void( EnemyBullet::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}