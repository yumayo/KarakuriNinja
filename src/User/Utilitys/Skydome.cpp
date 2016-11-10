#include "Skydome.h"

#include "GlobalData.hpp"

#include "cinder/gl/gl.h"

#include "cinder/Vector.h"

namespace User
{
    using namespace cinder;

    Skydome::Skydome( std::string const & texturePath, float scale )
        : domeTexture( &GData::FindTexture( texturePath ) )
        , scale( scale )
    {
    }
    void Skydome::Draw( cinder::CameraPersp const & camera )
    {
        glCullFace( GL_FRONT );
        gl::color( Color::white( ) );
        domeTexture->bind( );
        gl::drawSphere( camera.getEyePoint( ), scale, 100 );
        domeTexture->unbind( );
        glCullFace( GL_BACK );
    }
}