#pragma once

#include <memory>

#include "cinder/Vector.h"

namespace cinder
{
    namespace app
    {
        class MouseEvent;
    }
}

namespace Lib
{
    class InputMouse
    {
        struct _m_InputMouse;
        std::shared_ptr<_m_InputMouse> member;
    public:
        InputMouse( );
    public:
        bool isPush( const int button ) const;
        bool isPress( const int button ) const;
        bool isPull( const int button ) const;
        void end( );
        void flushInput( ) const;
        const cinder::Vec2i& position( ) const;
        float wheel( ) const;
    private:
        void mouseDown( cinder::app::MouseEvent& event );
        void mouseUp( cinder::app::MouseEvent& event );
        void mouseMove( cinder::app::MouseEvent& event );
        void mouseDrag( cinder::app::MouseEvent& event );
        void mouseWheel( cinder::app::MouseEvent& event );
    private:
        void mouseDownEnum( const int button );
        void mouseUpEnum( const int button );
    private:
        friend class InputTask;
    };
}