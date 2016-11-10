# include "EnemyBulletBase.h"

# include "cinder/ImageIo.h"
# include "cinder/app/App.h"
#include "EnemyBulletTexture.h"

namespace User
{
    using namespace cinder;

    EnemyBulletTexture::EnemyBulletTexture( cinder::Vec3f position, cinder::Vec3f speed, std::string const & path )
        : EnemyBulletBase( position, speed )
        , texture( loadImage( app::loadAsset( path ) ) )
    {
    }

    void EnemyBulletTexture::draw( )
    {
        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );
        texture.bind( );
        gl::pushModelView( );
        gl::rotate( Vec3f( 0, 0, 180 ) );
        gl::color( Color::white( ) );
        gl::drawCube( Vec3f::zero( ), object.Size( ) );
        gl::popModelView( );
        texture.unbind( );
    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedCube( Vec3f::zero( ), object.Size( ) );
    #endif // _DEBUG
        gl::popModelView( );
    }
}