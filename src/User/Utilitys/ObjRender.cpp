#include "ObjRender.h"
#include "cinder/gl/gl.h"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    ObjRender::ObjRender( std::string const & texturePath, std::string const & objPath )
        : position( cinder::Vec3f::zero( ) )
        , scale( cinder::Vec3f::one( ) )
        , rotate( cinder::Vec3f::zero( ) )
    {
        texture = &GData::FindTexture( texturePath );
        mesh = &GData::FindObj( objPath );
    }
    void ObjRender::Draw( )
    {
        gl::pushModelView( );

        gl::translate( position );
        gl::rotate( rotate );
        gl::scale( scale );
        gl::color( Color::white( ) );
        texture->bind( );
        gl::draw( *mesh );
        texture->unbind( );

        gl::popModelView( );
    }
}