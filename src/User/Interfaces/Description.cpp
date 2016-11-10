#include "Description.h"

# include "../Utilitys/Yumayo.h"

# include "Framework.hpp"

# include "GlobalData.hpp"
# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    Description::Description( )
    {
        touchToNext = &GData::FindTexture( "TouchToNext.png" );
        kougeki = &GData::FindTexture( "kougeki.png" );
        bougyo = &GData::FindTexture( "bougyo.png" );
    }
    void Description::Draw( cinder::Vec2f position )
    {
        // 説明用のセリフが無かった場合は描画しません。
        if ( TRData::Serif( ).empty( ) ) return;

        frame += 1;

        switch ( mode )
        {
        case User::Description::NORMAL:
            DrawNORMAL( position );
            break;
        case User::Description::KOUGEKI:
            DrawKOUGEKI( position );
            break;
        case User::Description::BOUGYO:
            DrawBOUGYO( position );
            break;
        default:
            break;
        }
    }
    void Description::DrawNORMAL( cinder::Vec2f position )
    {
        int index = ( frame / 30 ) % ( 2 );
        Vec2f cutSize( 768, 256 );
        int num = touchToNext->getWidth( ) / cutSize.x;
        float x_offset = ( index % num ) * cutSize.x;
        float y_offset = ( index / num ) * cutSize.y;
        Vec2f offset( x_offset, y_offset );

        DrawCutTexture( touchToNext, position, cutSize / 2, offset, cutSize );
    }
    void Description::DrawKOUGEKI( cinder::Vec2f position )
    {
        Vec2f cutSize( 768, 256 );
        Vec2f offset( Vec2f::zero( ) );

        DrawCutTexture( kougeki, position, cutSize / 2, offset, cutSize );
    }
    void Description::DrawBOUGYO( cinder::Vec2f position )
    {
        Vec2f cutSize( 768, 256 );
        Vec2f offset( Vec2f::zero( ) );

        DrawCutTexture( bougyo, position, cutSize / 2, offset, cutSize );
    }
    void Description::DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize )
    {
        gl::pushModelView( );
        gl::translate( -offset + position );
        gl::draw( *texture, Area( offset, offset + cutSize ), Area( offset, offset + size ) );
        gl::popModelView( );
    }
}