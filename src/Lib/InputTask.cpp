#include "InputTask.h"

#include "InputGamepadTask.h"

namespace Lib
{
    using namespace cinder;
    using namespace cinder::app;

    InputTask & InputTask::Geti( )
    {
        static InputTask insntans;
        return insntans;
    }
    void InputTask::setup( )
    {
        gamePad.setup( );
    }
    void InputTask::shutdown( )
    {
        gamePad.shutdown( );
    }
    void InputTask::update( )
    {
        gamePad.update( );
    }
    void InputTask::mouseDown( cinder::app::MouseEvent& event )
    {
        mouse.mouseDown( event );
    }
    void InputTask::mouseUp( cinder::app::MouseEvent& event )
    {
        mouse.mouseUp( event );
    }
    void InputTask::mouseWheel( cinder::app::MouseEvent& event )
    {
        mouse.mouseWheel( event );
    }
    void InputTask::mouseMove( cinder::app::MouseEvent& event )
    {
        mouse.mouseMove( event );
    }
    void InputTask::mouseDrag( cinder::app::MouseEvent& event )
    {
        mouse.mouseDrag( event );
    }
    void InputTask::keyDown( cinder::app::KeyEvent& event )
    {
        key.keyDown( event );
    }
    void InputTask::keyUp( cinder::app::KeyEvent& event )
    {
        key.keyUp( event );
    }
    void InputTask::touchesBegan( cinder::app::TouchEvent& event )
    {
        touch.touchesBegan( event );
    }
    void InputTask::touchesMoved( cinder::app::TouchEvent& event )
    {
        touch.touchesMoved( event );
    }
    void InputTask::touchesEnded( cinder::app::TouchEvent& event )
    {
        touch.touchesEnded( event );
    }
}