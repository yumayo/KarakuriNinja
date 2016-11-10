#include "KarakuriBase.h"

#include "cinder/gl/gl.h"

#include "../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    KarakuriBase::KarakuriBase( cinder::JsonTree const& params )
        : object( params )
    {
        Direction( Vec3f( 0, 0, -1 ) );
    }

    void KarakuriBase::update( int count )
    {
        object.Move( count );
    }

    void KarakuriBase::draw( )
    {
        gl::color( ColorA( 1, 1, 0, 1 ) );
        gl::drawVector( object.Position( ), object.Position( ) + direction * 0.1 );

        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( Rotate( ) );
        gl::color( ColorA::white( ) );
        object.bind( );
        gl::pushModelView( );
        gl::rotate( Vec3f( 0, 0, 180 ) );
        gl::drawCube( Vec3f::zero( ), object.Size( ) );
        gl::popModelView( );
        object.unbind( );
    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedCube( Vec3f::zero( ), object.Size( ) );
    #endif // _DEBUG
        gl::popModelView( );
    }
}
