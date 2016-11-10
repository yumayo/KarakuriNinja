# include "BulletEnemy.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

# include "cinder/ImageIo.h"
# include "cinder/app/App.h"

namespace User
{
    using namespace cinder;
    BulletEnemy::BulletEnemy( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera )
        , time( 0 )
        , nextJumpTime( randInt( 60, 120 ) )
        , texture( loadImage( app::loadAsset( "BulletEnemy.png" ) ) )
    {

    }
    void BulletEnemy::update( )
    {
        EnemyBase::update( );

        Thinking( );
        Advance( );

        // デバッグダメージ
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Hit( 0.3 );
    }
    void BulletEnemy::draw( )
    {
        gl::color( ColorA( 1, 1, 0, 1 ) );
        gl::drawVector( object.Position( ), object.Position( ) + object.Direction( ) * 0.1 );

        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );
        texture.bind( );
        gl::pushModelView( );
        gl::rotate( Vec3f( 0, 0, 180 ) );
        gl::color( HitColor( ) );
        gl::drawCube( Vec3f::zero( ), object.Size( ) );
        gl::popModelView( );
        texture.unbind( );
    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedCube( Vec3f::zero( ), object.Size( ) );
    #endif // _DEBUG
        gl::popModelView( );
    }
    bool BulletEnemy::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    bool BulletEnemy::IsAction( )
    {
        return nextJumpTime < time;
    }
    bool BulletEnemy::IsThink( )
    {
        return IsLive( ) && isLanding;
    }
    void BulletEnemy::Thinking( )
    {
        if ( IsThink( ) ) time += 1;
    }
    void BulletEnemy::Advance( )
    {
        // 行動時間になったら弾を投げます。
        if ( IsAction( ) )
        {
            BulletCreate( EnemyBulletTexture( object.Position( ), object.Direction( ) * 0.02F, "shuriken2.png" ) );

            time = 0;
            nextJumpTime = randInt( 120, 300 );
        }
    }
}