#pragma once

#include <memory>

namespace cinder
{
    namespace app
    {
        class KeyEvent;
    }
}

namespace Lib
{
    class InputKey
    {
        struct _m_InputKey;
        std::shared_ptr<_m_InputKey> member;
    public:
        InputKey( );
    public:
        bool isPush( const int key ) const;
        bool isPress( const int key ) const;
        bool isPull( const int key ) const;
        void end( ) const;
        void flushInput( ) const;
    private:
        void keyDown( cinder::app::KeyEvent& event );
        void keyUp( cinder::app::KeyEvent& event );
    private:
        void keyDownEnum( const int key );
        void keyUpEnum( const int key );
    private:
        friend class InputTask;
    };
}