#include "InputKey.h"

#include "cinder/app/KeyEvent.h"

#include <set>

namespace Lib
{
    using namespace cinder;
    using namespace cinder::app;

    struct InputKey::_m_InputKey
    {
        std::set<int> push;
        std::set<int> press;
        std::set<int> pull;
    };

    InputKey::InputKey( )
    {
        member = std::make_shared<InputKey::_m_InputKey>( );
    }
    bool InputKey::isPush( const int key ) const
    {
        return member->push.find( key ) != member->push.cend( );
    }
    bool InputKey::isPress( const int key ) const
    {
        return member->press.find( key ) != member->press.cend( );
    }
    bool InputKey::isPull( const int key ) const
    {
        return member->pull.find( key ) != member->pull.cend( );
    }
    void InputKey::end( ) const
    {
        flushInput( );
    }
    void InputKey::flushInput( ) const
    {
        member->push.clear( );
        member->pull.clear( );
    }
    void InputKey::keyDown( cinder::app::KeyEvent& event )
    {
        keyDownEnum( event.getCode( ) );
    }
    void InputKey::keyUp( cinder::app::KeyEvent& event )
    {
        keyUpEnum( event.getCode( ) );
    }
    void InputKey::keyDownEnum( const int key )
    {
        if ( !isPress( key ) )
        {
            member->push.insert( key );
        }
        member->press.insert( key );
    }
    void InputKey::keyUpEnum( const int key )
    {
        member->press.erase( key );
        member->pull.insert( key );
    }
}