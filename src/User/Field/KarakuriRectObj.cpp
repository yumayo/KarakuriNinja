#include "KarakuriRectObj.h"

#include "cinder/gl/gl.h"

#include "../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    KarakuriRectObj::KarakuriRectObj( cinder::JsonTree const& params )
        : KarakuriActor( params )
    {

    }

    void KarakuriRectObj::Update( int count )
    {
        KarakuriActor::Update( count );
    }

    void KarakuriRectObj::Draw( )
    {
        gl::disable( GL_CULL_FACE );

        gl::pushModelView( );

        gl::translate( Position( ) );
        gl::rotate( Rotate( ) );
        gl::color( ColorA::white( ) );
        bind( );
        gl::drawSolidRect( Rectf( -Scale( ).xy( ) / 2.0F, Scale( ).xy( ) / 2.0F ) );
        unbind( );

    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedRect( Rectf( -Scale( ).xy( ) / 2.0F, Scale( ).xy( ) / 2.0F ) );
    #endif // _DEBUG

        gl::popModelView( );

        gl::enable( GL_CULL_FACE );
    }
}
