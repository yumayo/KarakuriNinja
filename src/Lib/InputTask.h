#pragma once

#include "InputKey.h"
#include "InputMouse.h"
#include "InputTouch.h"
#include "InputGamepadTask.h"

namespace Lib
{
    class InputTask
    {
    public:
        static InputTask& Geti( );
    public:
        void setup( );
        void shutdown( );
    public:
        void update( );
    public:
        void mouseDown( cinder::app::MouseEvent& event );
        void mouseUp( cinder::app::MouseEvent& event );
        void mouseWheel( cinder::app::MouseEvent& event );
        void mouseMove( cinder::app::MouseEvent& event );
        void mouseDrag( cinder::app::MouseEvent& event );
        void keyDown( cinder::app::KeyEvent& event );
        void keyUp( cinder::app::KeyEvent& event );
        void touchesBegan( cinder::app::TouchEvent& event );
        void touchesMoved( cinder::app::TouchEvent& event );
        void touchesEnded( cinder::app::TouchEvent& event );
    private:
        InputKey key;
        InputMouse mouse;
        InputTouch touch;
        InputGamepadTask gamePad;
    private:
        friend class Input;
    };
}