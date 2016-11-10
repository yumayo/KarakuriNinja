#pragma once

#include <memory>
#include <string>
#include <vector>

struct Gamepad_device;

namespace Lib
{
    class InputGamepadTask
    {
        struct _m_InputGamepadTask;
        static ::std::shared_ptr<_m_InputGamepadTask> member;
    public:
        static void setup();
        static void update();
        static void shutdown();
    public:
        static bool isPush(const ::std::string& handle, const size_t buttonID);
        static bool isPress(const ::std::string& handle, const size_t buttonID);
        static bool isPull(const ::std::string& handle, const size_t buttonID);
        static float axis(const ::std::string& handle, const size_t buttonID);
    public:
        static bool isPush(const size_t handle, const size_t buttonID);
        static bool isPress(const size_t handle, const size_t buttonID);
        static bool isPull(const size_t handle, const size_t buttonID);
        static float axis(const size_t handle, const size_t buttonID);
    public:
        static void end( );
        static void flushInput();
    public:
        static const ::std::vector<::std::string> GetHandleIDs();
    private:
        static void onButtonDown(Gamepad_device * device, unsigned int buttonID);
        static void onButtonUp(Gamepad_device * device, unsigned int buttonID);
        static void onAxisMoved(Gamepad_device * device, unsigned int axisID, float value);
        static void onDeviceAttached(Gamepad_device * device);
        static void onDeviceRemoved(Gamepad_device * device);
    private:
        friend class InputGamepadLib;
    };
}