#pragma once

#include "cinder/Vector.h"
#include <string>
#include <vector>

#include "cinder/app/AppNative.h"

namespace Lib
{
    class Input
    {
    public:
        static Input& Geti( )
        {
            static Input instans;
            return instans;
        }
    public:
        void setup( cinder::app::AppNative *app );
    public:
        cinder::app::AppNative& GetApp( );
    private:
        cinder::app::AppNative *app;
    public: // マウス
        bool isPushButton( const int button ) const;
        bool isPressButton( const int button ) const;
        bool isPullButton( const int button ) const;
        const cinder::Vec2i& mousePosition( ) const;
        float mouseWheel( ) const;
    public: // キーボード
        bool isPushKey( const int key ) const;
        bool isPressKey( const int key ) const;
        bool isPullKey( const int key ) const;
    public: // ゲームパッド
        bool isPushPadButton( const std::string& handle, const size_t buttonID ) const;
        bool isPressPadButton( const std::string& handle, const size_t buttonID ) const;
        bool isPullPadButton( const std::string& handle, const size_t buttonID ) const;
        float padAxis( const std::string& handle, const size_t buttonID ) const;
        bool isPushPadButton( const size_t handle, const size_t buttonID ) const;
        bool isPressPadButton( const size_t handle, const size_t buttonID ) const;
        bool isPullPadButton( const size_t handle, const size_t buttonID ) const;
        float padAxis( const size_t handle, const size_t buttonID ) const;
        const std::vector<std::string> GetPadHandleIDs( ) const;
    public: // スクリーンタッチ
        bool isPushTouch( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const;
        bool isPressTouch( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const;
        bool isPullTouch( const uint32_t handle, cinder::app::TouchEvent::Touch& touch ) const;
        const std::vector<uint32_t> GetTouchHandleIDs( ) const;
        const cinder::app::TouchEvent::Touch& touch( ) const;
    public: 
        void flushInput( ) const;
    public:
        void begin( );
        void end( );
    };
}