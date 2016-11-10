#pragma once

#include <memory>
#include <vector>

#include "ZKOOHand.h"

namespace User
{
    class InputZKOO
    {
        struct _m_InputZKOO;
        std::shared_ptr<_m_InputZKOO> member;
    public:
        InputZKOO( );
    public:
        bool isPush( const uint32_t handle, ZKOOHand& touch ) const;
        bool isPress( const uint32_t handle, ZKOOHand& touch ) const;
        bool isPull( const uint32_t handle, ZKOOHand& touch ) const;
        void flushInput( ) const;
        const std::vector<uint32_t>& GetHandleIDs( ) const;
    public:
        void end( ) const;
        void update( );
    private:
        void ZKOOUpdate( );
    private:
        bool IsNowActiveInput( const uint32_t handle );
        bool IsPrevActiveInput( const uint32_t handle );
    private:
        void push( uint32_t handle, ZKOOHand event );
        void press( uint32_t handle, ZKOOHand event );
        void pull( uint32_t handle, ZKOOHand event );
    };
}