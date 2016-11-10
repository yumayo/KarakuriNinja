#pragma once

#include <memory>
#include <vector>

#include "cinder/app/TouchEvent.h"

namespace Lib
{
    class InputTouch
    {
        struct _m_InputTouch;
        std::shared_ptr<_m_InputTouch> member;
    public:
        InputTouch( );
    public:
        bool isPush( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const;
        bool isPress( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const;
        bool isPull( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const;
        void end( ) const;
        void flushInput( ) const;
        const std::vector<uint32_t>& GetHandleIDs( ) const;
    private:
        void touchesBegan( cinder::app::TouchEvent& event );
        void touchesMoved( cinder::app::TouchEvent& event );
        void touchesEnded( cinder::app::TouchEvent& event );
    private:
        friend class InputTask;
    };
}