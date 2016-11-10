#include "Talk.h"

# include "Framework.hpp"

# include "GlobalData.hpp"
# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    Talk::Talk( )
        : font( u8"メイリオ", 48 )
    {
        int index = 1;
        messageBoxLeft = &GData::FindTexture( "UI/serihu" + std::to_string( index++ ) + ".png" );
        messageBoxCenter = &GData::FindTexture( "UI/serihu" + std::to_string( index++ ) + ".png" );
        messageBoxRight = &GData::FindTexture( "UI/serihu" + std::to_string( index++ ) + ".png" );
    }
    bool Talk::IsAnyPullTouch( )
    {
        auto touch = inputs.touch( );
        for ( auto& obj : inputs.GetTouchHandleIDs( ) )
        {
            if ( inputs.isPullTouch( obj, touch ) )
            {
                return true;
            }
        }
        return false;
    }
    bool Talk::IsAnyPushTouch( )
    {
        auto touch = inputs.touch( );
        for ( auto& obj : inputs.GetTouchHandleIDs( ) )
        {
            if ( inputs.isPushTouch( obj, touch ) )
            {
                return true;
            }
        }
        return false;
    }
    bool Talk::IsAnyPressTouch( )
    {
        auto touch = inputs.touch( );
        for ( auto& obj : inputs.GetTouchHandleIDs( ) )
        {
            if ( inputs.isPressTouch( obj, touch ) )
            {
                return true;
            }
        }
        return false;
    }
    void Talk::Update( )
    {
        if ( IsAnyPushTouch( ) ) TRData::PopFrontSerif( );
    }
    void Talk::Draw( cinder::Vec2f position )
    {
        if ( TRData::Serif( ).empty( ) )return;

        auto textSize = font.BoundingBox( TRData::Serif( ) ).getSize( );
        auto messageBoxHeight = messageBoxLeft->getHeight( );
        auto rectLeft = Rectf( Vec2f::zero( ), messageBoxLeft->getSize( ) );
        auto rectCenter = Rectf( Vec2f::zero( ), Vec2f( textSize.x, messageBoxHeight ) );
        auto rectRight = Rectf( Vec2f::zero( ), messageBoxRight->getSize( ) );

        int ue = 5;
        int inMessageHeight = 63;
        int fontStartHeightPosition = ue + ( inMessageHeight - textSize.y ) / 2;

        // メッセージボックスの表示
        gl::pushModelView( );

        gl::translate( position + Vec2f( 0, -messageBoxHeight ) );
        messageBoxLeft->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectLeft );
        messageBoxLeft->unbind( );

        gl::translate( Vec2f( rectLeft.getWidth( ), 0 ) );
        messageBoxCenter->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectCenter );
        messageBoxCenter->unbind( );

        gl::pushModelView( );
        gl::translate( 0, fontStartHeightPosition );
        font.Draw( TRData::Serif( ), Vec2f::zero( ), Color::black( ), Fonts::Mode::LEFTUP );
        gl::popModelView( );

        gl::translate( Vec2f( rectCenter.getWidth( ), 0 ) );
        messageBoxRight->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectRight );
        messageBoxRight->unbind( );

        gl::popModelView( );
    }
}