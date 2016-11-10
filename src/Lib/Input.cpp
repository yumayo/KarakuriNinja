#include "Input.h"
#include "InputTask.h"

namespace Lib
{
    Input & Input::Geti( )
    {
        static Input instans;
        return instans;
    }
    bool Input::isPushButton( const int button ) const
    {
        return InputTask::Geti( ).mouse.isPush( button );
    }
    bool Input::isPressButton( const int button ) const
    {
        return InputTask::Geti( ).mouse.isPress( button );
    }
    bool Input::isPullButton( const int button ) const
    {
        return InputTask::Geti( ).mouse.isPull( button );
    }
    const cinder::Vec2i& Input::mousePosition( ) const
    {
        return InputTask::Geti( ).mouse.position( );
    }
    float Input::mouseWheel( ) const
    {
        return InputTask::Geti( ).mouse.wheel( );
    }
    bool Input::isPushKey( const int key ) const
    {
        return InputTask::Geti( ).key.isPush( key );
    }
    bool Input::isPressKey( const int key ) const
    {
        return InputTask::Geti( ).key.isPress( key );
    }
    bool Input::isPullKey( const int key ) const
    {
        return InputTask::Geti( ).key.isPull( key );
    }
    bool Input::isPushPadButton( const std::string & handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.isPush( handle, buttonID );
    }
    bool Input::isPressPadButton( const std::string & handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.isPress( handle, buttonID );
    }
    bool Input::isPullPadButton( const std::string & handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.isPull( handle, buttonID );
    }
    float Input::padAxis( const std::string & handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.axis( handle, buttonID );
    }
    bool Input::isPushPadButton( const size_t handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.isPush( handle, buttonID );
    }
    bool Input::isPressPadButton( const size_t handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.isPress( handle, buttonID );
    }
    bool Input::isPullPadButton( const size_t handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.isPull( handle, buttonID );
    }
    float Input::padAxis( const size_t handle, const size_t buttonID ) const
    {
        return InputTask::Geti( ).gamePad.axis( handle, buttonID );
    }
    const std::vector<std::string> Input::GetPadHandleIDs( ) const
    {
        return InputTask::Geti( ).gamePad.GetHandleIDs( );
    }
    bool Input::isPushTouch( const uint32_t handle, cinder::app::TouchEvent::Touch & touch ) const
    {
        return InputTask::Geti( ).touch.isPush( handle, touch );
    }
    bool Input::isPressTouch( const uint32_t handle, cinder::app::TouchEvent::Touch & touch ) const
    {
        return InputTask::Geti( ).touch.isPress( handle, touch );
    }
    bool Input::isPullTouch( const uint32_t handle, cinder::app::TouchEvent::Touch & touch ) const
    {
        return InputTask::Geti( ).touch.isPull( handle, touch );
    }
    const std::vector<uint32_t> Input::GetTouchHandleIDs( ) const
    {
        return InputTask::Geti( ).touch.GetHandleIDs( );
    }
    const cinder::app::TouchEvent::Touch& Input::touch( ) const
    {
        static cinder::app::TouchEvent::Touch touch;
        return touch;
    }
    void Input::flushInput( ) const
    {
        InputTask::Geti( ).mouse.flushInput( );
        InputTask::Geti( ).key.flushInput( );
        InputTask::Geti( ).touch.flushInput( );
        InputTask::Geti( ).gamePad.flushInput( );
    }
    void Input::begin( )
    {
        /* nothing */
    }
    void Input::end( )
    {
        InputTask::Geti( ).mouse.end( );
        InputTask::Geti( ).key.end( );
        InputTask::Geti( ).touch.end( );
        InputTask::Geti( ).gamePad.end( );
    }
}