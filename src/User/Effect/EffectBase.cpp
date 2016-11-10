#include "EffectBase.h"

#include "Framework.hpp"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EffectBase::EffectBase( std::string const & path, cinder::Vec2f position, cinder::Vec2f cutSize, Mode mode )
        : texture( &GData::FindTexture( path ) )
        , frame( 0 )
        , speed( 5 )
        //, position( position )
        //, cutSize( cutSize )
        , isActive( true )
        //, maxIndex( 0 )
        , mode( mode )
    {
        Position( position );
        CutSize( cutSize );
    }
    void EffectBase::Update( )
    {
        if ( NowIndex( ) == maxIndex - 1 ) Erase( );

        frame += 1;
    }
    void EffectBase::Draw( cinder::CameraPersp const& camera )
    {
        auto cutPosition = TextureCutPosition( NowIndex( ) );
        DrawCutTexture( cutPosition );
    }
    void EffectBase::Position( cinder::Vec2f position )
    {
        this->position = position;
    }
    void EffectBase::CutSize( cinder::Vec2f cutSize )
    {
        this->cutSize = cutSize;
        int x_num = texture->getWidth( ) / cutSize.x;
        int y_num = texture->getHeight( ) / cutSize.y;
        maxIndex = x_num * y_num;
    }
    bool EffectBase::IsActive( )
    {
        return isActive;
    }
    int EffectBase::NowIndex( )
    {
        return ( frame / speed ) % ( maxIndex );
    }
    cinder::Vec2f EffectBase::TextureCutPosition( int index )
    {
        int num = texture->getWidth( ) / cutSize.x;

        // êÿÇËî≤Ç´à íuÇåvéZ
        float x_offset = ( index % num ) * cutSize.x;
        float y_offset = ( index / num ) * cutSize.y;

        return cinder::Vec2f( x_offset, y_offset );
    }
    void EffectBase::DrawCutTexture( cinder::Vec2f cutPosition )
    {
        gl::pushModelView( );
        switch ( mode )
        {
        case User::EffectBase::Mode::LEFTUP:
            gl::translate( position - cutPosition );
            break;
        case User::EffectBase::Mode::CENTERCENTER:
            gl::translate( position - cutSize / 2.0F - cutPosition );
            break;
        default:
            break;
        }
        gl::color( Color::white( ) );
        gl::draw( *texture, Area( cutPosition, cutPosition + cutSize ), Area( cutPosition, cutPosition + cutSize ) );
        gl::popModelView( );
    }
    void EffectBase::Erase( )
    {
        isActive = false;
    }
}