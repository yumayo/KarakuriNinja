#include "Interface.h"
#include "Framework.hpp"
#include "cinder/ImageIo.h"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    Interface::Interface( )
        : APEdge( &GData::FindTexture( "UI/APEdge.png" ) )
        , HPEdge( &GData::FindTexture( "UI/HPEdge.png" ) )
        , score( &GData::FindTexture( "UI/Score.png" ) )
        , APEdgeBase( &GData::FindTexture( "UI/APEdgeBase.png" ) )
        , HPEdgeBase( &GData::FindTexture( "UI/HPEdgeBase.png" ) )
        , font( u8"ƒƒCƒŠƒI", 85 )
        , scorePoint( 0 )
    { }

    void Interface::draw( float APNormalized, float HPNormalized )
    {
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );

        gl::color( Color::white( ) );

        auto translateScorePosition = Vec2f( -11, -26 );
        textureDraw( *score, rightDown + Vec2f( -score->getWidth( ), 0 ) + translateScorePosition );
        font.Draw( std::to_string( scorePoint ), rightDown + Vec2f( -score->getWidth( ) / 2.0F + 32, -score->getHeight( ) / 2.0F - 32 ) + translateScorePosition, Color::white( ), Fonts::Mode::RIGHTUP );

        textureDraw( *HPEdge, leftDown );
        textureDraw( *HPEdgeBase, leftDown,
                     Area( 225, 0, 639, HPEdgeBase->getHeight( ) ), HPNormalized );

        auto translateAPEdgePosition = Vec2f( 226, -101 );
        textureDraw( *APEdge, leftDown + translateAPEdgePosition );
        textureDraw( *APEdgeBase, leftDown + translateAPEdgePosition,
                     Area( 15, 0, 340, APEdgeBase->getHeight( ) ), APNormalized );

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