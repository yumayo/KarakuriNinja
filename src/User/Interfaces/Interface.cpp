#include "Interface.h"
#include "Framework.hpp"
#include "cinder/ImageIo.h"

namespace User
{
    using namespace cinder;

    Interface::Interface( )
        : playerIconEdge( loadImage( app::loadAsset( "PlayerIconEdge.png" ) ) )
        , APEdge( loadImage( app::loadAsset( "APEdge.png" ) ) )
        , HPEdge( loadImage( app::loadAsset( "HPEdge.png" ) ) )
        , score( loadImage( app::loadAsset( "score.png" ) ) )
        , APEdgeBase( loadImage( app::loadAsset( "APEdgeBase.png" ) ) )
        , HPEdgeBase( loadImage( app::loadAsset( "HPEdgeBase.png" ) ) )
    { }

    void Interface::draw( float APNormalized, float HPNormalized )
    {
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );

        auto translatePlayerIconEdge = Vec2f( 10 + 128, -10 - 128 );

        gl::color( Color::white( ) );

        textureDraw( APEdgeBase, leftDown + translatePlayerIconEdge + Vec2f( APEdgeBase.getWidth( ) / 2.0F, 0 ) + Vec2f( 128, -64 ),
                     Area( 94, 0, 314, APEdgeBase.getHeight( ) ), APNormalized );
        textureDraw( APEdge, leftDown + translatePlayerIconEdge + Vec2f( APEdge.getWidth( ) / 2.0F, 0 ) + Vec2f( 128, -64 ) );

        textureDraw( HPEdgeBase, leftDown + translatePlayerIconEdge + Vec2f( HPEdgeBase.getWidth( ) / 2.0F, 0 ) + Vec2f( 128, 64 ),
                     Area( 10, 0, 730, HPEdgeBase.getHeight( ) ), HPNormalized );
        textureDraw( HPEdge, leftDown + translatePlayerIconEdge + Vec2f( HPEdge.getWidth( ) / 2.0F, 0 ) + Vec2f( 128, 64 ) );

        textureDraw( playerIconEdge, leftDown + translatePlayerIconEdge );

        textureDraw( score, rightDown + Vec2f( -512, -10 - 128 ) );
    }

    void Interface::update( )
    {

    }

    void Interface::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position );
        gl::draw( texture );
        texture.unbind( );
        gl::popModelView( );
    }

    void Interface::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Area area, float size )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position );
        gl::draw( texture,
                  Area( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ),
                  Rectf( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ) );
        texture.unbind( );
        gl::popModelView( );
    }
}