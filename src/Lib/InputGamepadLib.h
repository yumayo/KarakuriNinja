#pragma once

class InputGamepadLib
{
public:
    static void setup();
    static void shutdown();
    static void update();
public:
    static void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
    static void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
    static void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context);
    static void onDeviceAttached(struct Gamepad_device * device, void * context);
    static void onDeviceRemoved(struct Gamepad_device * device, void * context);
private:
    enum class define
    {
        POLL_ITERATION_INTERVAL = 30
    };
};