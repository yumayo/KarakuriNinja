#include "GameStartGauge.h"

#include "GlobalData.hpp"

#include "Hirasawa.h"

namespace User
{
    using namespace cinder;

    GameStartGauge::GameStartGauge( )
        : countFrame( 60 * 2 )
        , effectCountFrame( 60 )
    {
        effectColorA = ColorA( 1, 1, 1, 0 );
        wakuTexture = &GData::FindTexture( "Textures/spwaku.png" );
        gaugeTexture = &GData::FindTexture( "Textures/spgage2.png" );
        nigiriTexture = &GData::FindTexture( "Textures/nigiritudukete.png" );
        size = nigiriTexture->getSize( );
        effectSize = Vec2f( 0, 0 );
        gaugeSize = Vec2f( size.x, size.y - 20 );
    }
    int GameStartGauge::GetLoadFrame( )
    {
        return countFrame.GetMaxFrame( );
    }
    bool GameStartGauge::IsEnd( )
    {
        return countFrame.IsMax( );
    }
    void GameStartGauge::Update( bool isSuccess )
    {
        if ( isMaxed ) return;
        if ( isSuccess )
        {
            countFrame.Update( );
            if ( countFrame.IsMax( ) ) isMaxed = true;
        }
        else
        {
            countFrame = CountFrame( 60 * 2 );
        }
    }
    void GameStartGauge::Draw( )
    {
        if ( isMaxed )
        {
            effectColorA.a = EasingReturn( effectCountFrame.NormalizedRectSizeFrame( ), 0, 1 );
            effectSize.x = EasingReturn( effectCountFrame.NormalizedRectSizeFrame( ), 0, 30 );
            effectSize.y = EasingReturn( effectCountFrame.NormalizedRectSizeFrame( ), 0, 30 );
            effectCountFrame.Update( );
        }

        gl::color( Color::white( ) );

        gl::pushModelView( );
        gl::draw( *nigiriTexture, Rectf( Vec2f::zero( ), size ) );
        gl::translate( 0, size.y - 20 );

        auto cutSize = Vec2f( gaugeSize.x * countFrame.NormalizedRectSizeFrame( ), gaugeSize.y );
        DrawCutTexture( gaugeTexture, Vec2f::zero( ), cutSize, Vec2f::zero( ), cutSize );
        gl::draw( *wakuTexture, Rectf( Vec2f::zero( ) - effectSize, gaugeSize + effectSize ) );
        gl::color( effectColorA );
        if ( isMaxed ) gl::drawSolidRect( Rectf( Vec2f::zero( ) - effectSize, gaugeSize + effectSize ) );
        gl::color( Color::white( ) );
        gl::popModelView( );
    }
    void GameStartGauge::DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize )
    {
        gl::pushModelView( );
        gl::translate( -offset + position );
        gl::color( Color::white( ) );
        gl::draw( *texture, Area( offset, offset + cutSize ), Area( offset, offset + size ) );
        gl::popModelView( );
    }
}