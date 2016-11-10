#include "Talk.h"

# include "Framework.hpp"
# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    Talk::Talk( )
        : font( u8"ÉÅÉCÉäÉI", 64 )
    {
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
        gl::pushModelView( );
        gl::translate( position );
        gl::drawSolidRect( font.BoundingBox( TRData::Serif( ) ) );
        font.Draw( TRData::Serif( ), Vec2f::zero( ), Color::black( ), Fonts::Mode::LEFTUP );
        gl::popModelView( );
    }
}