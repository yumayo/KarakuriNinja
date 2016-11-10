#include "Interface.h"
#include "Framework.hpp"
#include "cinder/ImageIo.h"

namespace User
{
    using namespace cinder;

    Interface::Interface( )
        : APEdge( loadImage( app::loadAsset( "UI/APEdge.png" ) ) )
        , HPEdge( loadImage( app::loadAsset( "UI/HPEdge.png" ) ) )
        , score( loadImage( app::loadAsset( "UI/Score.png" ) ) )
        , APEdgeBase( loadImage( app::loadAsset( "UI/APEdgeBase.png" ) ) )
        , HPEdgeBase( loadImage( app::loadAsset( "UI/HPEdgeBase.png" ) ) )
    { }

    void Interface::draw( float APNormalized, float HPNormalized )
    {
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );

        gl::color( Color::white( ) );

        auto translateScorePosition = Vec2f( -11, -26 );
        textureDraw( score, rightDown + Vec2f( -score.getWidth( ), 0 ) + translateScorePosition );

        textureDraw( HPEdge, leftDown );
        textureDraw( HPEdgeBase, leftDown,
                     Area( 224, 0, 864, HPEdgeBase.getHeight( ) ), HPNormalized );

        auto translateAPEdgePosition = Vec2f( 226, -101 );
        textureDraw( APEdge, leftDown + translateAPEdgePosition );
        textureDraw( APEdgeBase, leftDown + translateAPEdgePosition,
                     Area( 14, 0, 354, APEdgeBase.getHeight( ) ), APNormalized );
    }

    void Interface::update( )
    {

    }

    void Interface::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position + Vec2f( texture.getWidth( ) / 2.0F, -texture.getHeight( ) / 2.0F ) );
        gl::draw( texture );
        texture.unbind( );
        gl::popModelView( );
    }

    void Interface::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Area area, float size )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position + Vec2f( texture.getWidth( ) / 2.0F, -texture.getHeight( ) / 2.0F ) );
        gl::draw( texture,
                  Area( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ),
                  Rectf( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ) );
        texture.unbind( );
        gl::popModelView( );
    }
}