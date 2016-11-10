# include "SceneResult.h"
# include "SceneTitle.h"

using namespace ci;

namespace User
{
    SceneResult::SceneResult( )
    {

    }

    SceneResult::~SceneResult( )
    {

    }

    void SceneResult::resize( )
    {

    }

    void SceneResult::update( )
    {

    }
    void SceneResult::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }

    void SceneResult::select( )
    {
        create( new SceneTitle( ) );
    }

    void SceneResult::beginDrawUI( )
    {
        ci::gl::enable( GL_TEXTURE_2D );
        ci::gl::disable( GL_CULL_FACE );
        ci::gl::disable( GL_NORMALIZE );
        ci::gl::disable( GL_LIGHTING );
        ci::gl::enableAlphaBlending( );
        ci::gl::disableDepthRead( );
        ci::gl::disableDepthWrite( );

        ci::gl::setViewport( env.getWindowBounds( ) );
        ci::gl::setMatricesWindow( env.getWindowSize( ) );
        ci::gl::color( Color::white( ) );
    }
    void SceneResult::drawUI( )
    {

    }
    void SceneResult::endDrawUI( )
    {

    }
}