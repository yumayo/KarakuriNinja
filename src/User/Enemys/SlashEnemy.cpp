# include "SlashEnemy.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "cinder/ImageIo.h"
# include "cinder/app/App.h"
namespace User
{
    using namespace cinder;
    SlashEnemy::SlashEnemy( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera )
        , time( 0 )
        , nextJumpTime( randInt( 60, 120 ) )
        , texture( loadImage( app::loadAsset( "SlashEnemy.png" ) ) )
    {

    }
    void SlashEnemy::update( )
    {
        EnemyBase::update( );

        Thinking( );
        Advance( );

        // デバッグダメージ
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Hit( 0.3 );
    }
    void SlashEnemy::draw( )
    {
        static float rotate = 0; rotate += 1;

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
    bool SlashEnemy::Attack( const cinder::CameraPersp& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( );
        // エネミの場所がカメラよりも手前側だったら攻撃します。
        if ( cameraPos.dot( cameraPos - object.Position( ) ) < 0 ) return true;
        else return false;
    }
    bool SlashEnemy::IsAction( )
    {
        return nextJumpTime < time;
    }
    bool SlashEnemy::IsThink( )
    {
        return IsLive( ) && isLanding;
    }
    void SlashEnemy::Thinking( )
    {
        if ( IsThink( ) ) time += 1;
    }
    void SlashEnemy::Advance( )
    {
        // 行動時間になったらジャンプをします。
        if ( IsAction( ) )
        {
            object.Speed( Vec3f( 0, 0.1, 0 ) );
            isLanding = false;
            time = 0;
            nextJumpTime = randInt( 60, 120 );
        }
    }
}