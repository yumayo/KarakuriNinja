# include "EnemyBulletBase.h"

# include "cinder/ImageIo.h"
# include "cinder/app/App.h"
#include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EnemyBulletTexture::EnemyBulletTexture( cinder::Vec3f begin, cinder::Vec3f end, std::string const & path )
        : EnemyBulletBase( begin, end )
        , texture( &GData::FindTexture( path ) )
    {
    }

    EnemyBulletTexture::EnemyBulletTexture( cinder::Vec3f begin, cinder::Vec3f end, std::string const & path, int attackPoint )
        : EnemyBulletBase( begin, end, attackPoint )
        , texture( &GData::FindTexture( path ) )
    {
    }

    void EnemyBulletTexture::draw( )
    {
        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );

        gl::pushModelView( );
        gl::rotate( Vec3f( 0, 180, 180 ) );
        gl::color( Color::white( ) );
        texture->bind( );
        gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
        texture->unbind( );
        gl::popModelView( );

    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
    #endif // _DEBUG

        gl::popModelView( );
    }
}