#include "InputTouch.h"

#include <set>
#include <map>

namespace Lib
{
    using namespace cinder;
    using namespace cinder::app;

    struct InputTouch::_m_InputTouch
    {
        std::map<uint32_t, TouchEvent::Touch> push;
        std::map<uint32_t, TouchEvent::Touch> press;
        std::map<uint32_t, TouchEvent::Touch> pull;
        std::vector<uint32_t> handles;
    };

    InputTouch::InputTouch( )
    {
        member = std::make_shared<InputTouch::_m_InputTouch>( );
    }
    bool InputTouch::isPush( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const
    {
        auto itr = member->push.find( handle );
        if ( itr != member->push.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputTouch::isPress( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const
    {
        auto itr = member->press.find( handle );
        if ( itr != member->press.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputTouch::isPull( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const
    {
        auto itr = member->pull.find( handle );
        if ( itr != member->pull.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    void InputTouch::end( ) const
    {
        flushInput( );
        member->handles.clear( );
    }
    void InputTouch::flushInput( ) const
    {
        member->push.clear( );
        member->pull.clear( );
    }
    const std::vector<uint32_t>& InputTouch::GetHandleIDs( ) const
    {
        return member->handles;
    }
    void InputTouch::touchesBegan( cinder::app::TouchEvent & event )
    {
        for ( std::vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches( ).begin( ); touchIt != event.getTouches( ).end( ); ++touchIt )
        {
            member->push.insert( std::make_pair( touchIt->getId( ), *touchIt ) );
            member->press.insert( std::make_pair( touchIt->getId( ), *touchIt ) );

            member->handles.push_back( touchIt->getId( ) );
        }
    }
    void InputTouch::touchesMoved( cinder::app::TouchEvent & event )
    {
        for ( std::vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches( ).begin( ); touchIt != event.getTouches( ).end( ); ++touchIt )
        {
            member->press[touchIt->getId( )] = *touchIt;

            member->handles.push_back( touchIt->getId( ) );
        }
    }
    void InputTouch::touchesEnded( cinder::app::TouchEvent & event )
    {
        for ( std::vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches( ).begin( ); touchIt != event.getTouches( ).end( ); ++touchIt )
        {
            member->pull.insert( std::make_pair( touchIt->getId( ), *touchIt ) );
            member->press.erase( touchIt->getId( ) );

            member->handles.push_back( touchIt->getId( ) );
        }
    }
}