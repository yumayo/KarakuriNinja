#include "EffectScore.h"

#include "Framework.hpp"

#include "GlobalData.hpp"

#include "../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    EffectScore::EffectScore( cinder::Vec2f position, int scoreNum )
        : EffectBase( "Textures/score.png", position, 0.0F, Vec2f( 256, 256 ), Vec2f( 256, 256 ), Mode::LEFTUP )
        , font( u8"HGçsèëëÃ", 90 )
        , score( scoreNum )
    {
        size = font.BoundingBox( std::to_string( score ) ).getSize( );

    }
    void EffectScore::Update( )
    {
        if ( 30 < frame ) Erase( );

        frame += 1;

        if ( frame <= 30 * 0.2 )
        {
            alpha = EasingLinear( ( frame - ( 0 * 0 ) ) / ( 30 * 0.2 ), 0.0F, 1.0F );
        }
        else if ( ( 30 * 0.8 ) <= frame )
        {
            alpha = EasingLinear( ( frame - ( 30 * 0.8 ) ) / ( 30 * 0.2 ), 1.0F, 0.0F );
        }

        position.y -= 1;
    }
    void EffectScore::Draw( )
    {
        gl::pushModelView( );
        gl::translate( position - size / 2 );

        texture->enableAndBind( );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), size ) );
        texture->unbind( );

        font.Draw( std::to_string( score ), Vec2f::zero( ), ColorA( 1, 1, 1, alpha ) );

        gl::popModelView( );
    }
}