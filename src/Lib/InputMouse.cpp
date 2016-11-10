#include "InputMouse.h"

#include "cinder/app/MouseEvent.h"

#include <set>

namespace Lib
{
    using namespace cinder;
    using namespace cinder::app;

    struct InputMouse::_m_InputMouse
    {
        std::set<int> push;
        std::set<int> press;
        std::set<int> pull;
        Vec2i pos;
        float wheel;

        InputMouse::_m_InputMouse( ) : pos( Vec2i::zero( ) ), wheel( 0.0f ) {}
    };

    InputMouse::InputMouse( )
    {
        member = std::make_shared<InputMouse::_m_InputMouse>( );
    }
    bool InputMouse::isPush( const int button ) const
    {
        return member->push.find( button ) != member->push.cend( );
    }
    bool InputMouse::isPress( const int button ) const
    {
        return member->press.find( button ) != member->press.cend( );
    }
    bool InputMouse::isPull( const int button ) const
    {
        return member->pull.find( button ) != member->pull.cend( );
    }
    void InputMouse::end( )
    {
        flushInput( );
    }
    void InputMouse::flushInput( ) const
    {
        member->push.clear( );
        member->pull.clear( );
    }
    const Vec2i& InputMouse::position( ) const
    {
        return member->pos;
    }
    float InputMouse::wheel( ) const
    {
        return member->wheel;
    }
    void InputMouse::mouseDown( cinder::app::MouseEvent& event )
    {
        if ( event.isLeft( ) )
        {
            mouseDownEnum( event.LEFT_DOWN );
        }
        if ( event.isRight( ) )
        {
            mouseDownEnum( event.RIGHT_DOWN );
        }
    }
    void InputMouse::mouseUp( cinder::app::MouseEvent & event )
    {
        if ( event.isLeft( ) )
        {
            mouseUpEnum( event.LEFT_DOWN );
        }
        if ( event.isRight( ) )
        {
            mouseUpEnum( event.RIGHT_DOWN );
        }
    }
    void InputMouse::mouseMove( cinder::app::MouseEvent & event )
    {
        member->pos = event.getPos( );
    }
    void InputMouse::mouseDrag( cinder::app::MouseEvent & event )
    {
        member->pos = event.getPos( );
    }
    void InputMouse::mouseWheel( cinder::app::MouseEvent & event )
    {
        member->wheel = event.getWheelIncrement( );
    }
    void InputMouse::mouseDownEnum( const int button )
    {
        member->push.insert( button );
        member->press.insert( button );
    }
    void InputMouse::mouseUpEnum( const int button )
    {
        member->press.erase( button );
        member->pull.insert( button );
    }
}