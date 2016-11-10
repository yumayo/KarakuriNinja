# include "CPMeshFilter.hpp"

# include "cinder/gl/gl.h"

namespace User
{
    using namespace cinder;

    void null( )
    {
        ;
    }
    void sphere( )
    {
        gl::drawSphere( Vec3f::zero( ), 1.0f );
    }
    void cube( )
    {
        gl::drawCube( Vec3f::zero( ), Vec3f::one( ) );
    }
    void skydome( )
    {
        glCullFace( GL_FRONT );
        gl::disable( GL_LIGHTING );
        gl::color( Color::white( ) );
        gl::drawSphere( Vec3f::zero( ), 1.0f, 100 );
        gl::enable( GL_LIGHTING );
        glCullFace( GL_BACK );
    }
    void torus( )
    {
        gl::drawTorus( 1, 3 );
    }

    std::map<std::string, std::function<void( )>> CPMeshFilter::filters =
    {
        { "null", null },
        { "sphere", sphere },
        { "cube", cube },
        { "skydome", skydome },
        { "torus", torus }
    };

    CPMeshFilter::CPMeshFilter( const std::string & meshName )
    {
        auto itr = filters.find( meshName );
        if ( itr != filters.cend( ) ) meshFunction = itr->second;
        else  meshFunction = null;
    }
    void CPMeshFilter::draw( )
    {
        meshFunction( );
    }
}