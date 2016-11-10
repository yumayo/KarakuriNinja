#include "Combo.h"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    Combo::Combo( )
        : font( u8"HGçsèëëÃ", 85 )
        , backGround( &GData::FindTexture( "UI/ReamHammer.png" ) )
        , effect( "Textures/Effect/pipo-btleffect102c.png",
                  Vec2f::zero( ),
                  0.0F,
                  Vec2f( 480, 480 ),
                  Vec2f( 240, 240 ),
                  EffectBase::Mode::LEFTUP )
        , comboNumber( 0 )
        , maxComboNumber( comboNumber )
    {
    }
    void Combo::Update( )
    {
        if ( comboNumber == 0 ) return;

        effect.Update( );
    }
    void Combo::PlusCombo( int attackSuccessNum )
    {
        if ( 0 < attackSuccessNum )
        {
            comboNumber += attackSuccessNum;
            if ( maxComboNumber < comboNumber ) maxComboNumber = comboNumber;
        }
    }
    void Combo::ResetCombo( )
    {
        comboNumber = 0;
    }
    void Combo::Draw( cinder::Vec2f position )
    {
        if ( comboNumber != 0 )
        {
            gl::pushModelView( );
            gl::translate( position );

            effect.Draw( );

            gl::draw( *backGround );
            font.Draw( std::to_string( comboNumber ), Vec2f( 60, 95 ), Color::white( ) );
            gl::popModelView( );
        }
    }
}