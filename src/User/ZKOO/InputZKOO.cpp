#include "InputZKOO.h"
#include "ZKOOMaker.h"

#include <map>

namespace User
{
    using namespace cinder;

    struct InputZKOO::_m_InputZKOO
    {
        std::map<uint32_t, ZKOOHand> push;
        std::map<uint32_t, ZKOOHand> press;
        std::map<uint32_t, ZKOOHand> pull;
        std::vector<uint32_t> handles;

        // １フレーム前のZKOOの状態を保存しておきます。
        std::vector<ZKOOHand> prevZKOOHands;
    };

    InputZKOO::InputZKOO( )
    {
        member = std::make_shared<InputZKOO::_m_InputZKOO>( );
    }
    bool InputZKOO::isPush( const uint32_t handle, ZKOOHand& touch ) const
    {
        auto itr = member->push.find( handle );
        if ( itr != member->push.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputZKOO::isPress( const uint32_t handle, ZKOOHand& touch ) const
    {
        auto itr = member->press.find( handle );
        if ( itr != member->press.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputZKOO::isPull( const uint32_t handle, ZKOOHand& touch ) const
    {
        auto itr = member->pull.find( handle );
        if ( itr != member->pull.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    void InputZKOO::end( ) const
    {
        flushInput( );
        member->handles.clear( );
    }
    void InputZKOO::flushInput( ) const
    {
        member->push.clear( );
        member->pull.clear( );
    }
    const std::vector<uint32_t>& InputZKOO::GetHandleIDs( ) const
    {
        return member->handles;
    }
    void InputZKOO::update( )
    {
        ZKOOUpdate( );
    }
    void InputZKOO::ZKOOUpdate( )
    {
        for ( int i = 0; i < 2; ++i )
        {
            if ( IsPrevActiveInput( i ) == false && IsNowActiveInput( i ) == true )
            {
                push( i, ZKOOMaker::Geti( ).hands[i] );
                // push
            }
            if ( IsPrevActiveInput( i ) == true && IsNowActiveInput( i ) == true )
            {
                press( i, ZKOOMaker::Geti( ).hands[i] );
                // press
            }
            if ( IsPrevActiveInput( i ) == true && IsNowActiveInput( i ) == false )
            {
                pull( i, ZKOOMaker::Geti( ).hands[i] );
                // pull
            }
        }

        // ZKOOの状態を保存します。
        // このクラスは１フレーム前の状態と現在のフレームのZKOO状態を比べて判断しているので。
        member->prevZKOOHands = ZKOOMaker::Geti( ).hands;
    }

    bool InputZKOO::IsNowActiveInput( const uint32_t handle )
    {
        if ( ZKOOMaker::Geti( ).hands.size( ) == 2 )
        {
            return ZKOOMaker::Geti( ).hands[handle].IsTouching( );
        }
        return false;
    }

    bool InputZKOO::IsPrevActiveInput( const uint32_t handle )
    {
        if ( member->prevZKOOHands.size( ) == 2 )
        {
            return member->prevZKOOHands[handle].IsTouching( );
        }
        return false;
    }

    void InputZKOO::push( uint32_t handle, ZKOOHand event )
    {
        member->push.insert( std::make_pair( handle, event ) );
        member->press.insert( std::make_pair( handle, event ) );

        member->handles.push_back( handle );
    }
    void InputZKOO::press( uint32_t handle, ZKOOHand event )
    {
        member->press[handle] = event;

        member->handles.push_back( handle );
    }
    void InputZKOO::pull( uint32_t handle, ZKOOHand event )
    {
        member->pull.insert( std::make_pair( handle, event ) );
        member->press.erase( handle );

        member->handles.push_back( handle );
    }
}