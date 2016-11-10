#include "EffectCombo.h"

#include "Framework.hpp"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EffectCombo::EffectCombo( std::string const & path,
                              cinder::Vec2f position,
                              float depth,
                              cinder::Vec2f size,
                              cinder::Vec2f cutSize,
                              Mode mode,
                              cinder::Vec2f endpos )
        : EffectBase( path, position, depth, size, cutSize, mode )
        , t_( 0.0f )
        , endt_( 0.0f )
        , startpos_( position )
        , startsize_( size )
        , endpos_( endpos )
        , easingstart_( false )
    {
        Position( position );
        CutSize( cutSize );
    }
    void EffectCombo::Update( )
    {
        if ( frame >= 25 )
        {
            easingstart_ = true;
        }
        frame += 1;
        if ( easingstart_ )
        {
            Easing::tCount( t_, 0.8f );
            position.x = Easing::getEasing[Easing::CubicInOut]( t_, startpos_.x, endpos_.x );
            position.y = Easing::getEasing[Easing::CubicInOut]( t_, startpos_.y, endpos_.y );
            Vec2f endsize = Vec2f( 480, 480 );
            size.x = Easing::getEasing[Easing::CubicInOut]( t_, startsize_.x, endsize.x );
            size.y = Easing::getEasing[Easing::CubicInOut]( t_, startsize_.y, endsize.y );
        }
        if ( t_ >= 1.0f )
            Easing::tCount( endt_, 1.0f );
        if ( endt_ >= 1.0f )Erase( );

    }
    void EffectCombo::Draw( )
    {
        auto cutPosition = TextureCutPosition( NowIndex( ) );
        DrawCutTexture( cutPosition );
    }
    int EffectCombo::NowIndex( )
    {
        if ( t_ > 0.0f ) {
            return ( frame / speed ) % ( 3 ) + 4;
        }
        else {
            return ( frame / speed ) % ( maxIndex );
        }
    }
    void EffectCombo::DrawCutTexture( cinder::Vec2f cutPosition )
    {
        gl::color( ColorA( 1, 1, 1, 1.0 - 1.0f * endt_ ) );

        auto normalized = cutSize.normalized( );
        auto normalized_2 = normalized / 2.0F;

        gl::pushModelView( );
        gl::translate( position );
        gl::scale( size );

        switch ( mode )
        {
        case User::EffectBase::Mode::LEFTUP:
            gl::draw( *texture, Area( cutPosition, cutPosition + cutSize ), Rectf( Vec2f::zero( ), normalized ) );
            break;
        case User::EffectBase::Mode::CENTERCENTER:
            gl::draw( *texture, Area( cutPosition, cutPosition + cutSize ), Rectf( -normalized_2, normalized_2 ) );
            break;
        default:
            break;
        }

        gl::popModelView( );
    }
}