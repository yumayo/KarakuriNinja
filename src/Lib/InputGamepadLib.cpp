#include "InputGamepadLib.h"

extern "C"
{
# include "Gamepad.h"
}
# pragma comment(lib, "winmm.lib")
# if _DEBUG
#   pragma comment(lib, "Gamepad_d.lib")
# else
#   pragma comment(lib, "Gamepad.lib")
# endif

#include "InputGamepadTask.h"

void InputGamepadLib::setup()
{
    Gamepad_deviceAttachFunc(onDeviceAttached, (void *)0x1);
    Gamepad_deviceRemoveFunc(onDeviceRemoved, (void *)0x2);
    Gamepad_buttonDownFunc(onButtonDown, (void *)0x3);
    Gamepad_buttonUpFunc(onButtonUp, (void *)0x4);
    Gamepad_axisMoveFunc(onAxisMoved, (void *)0x5);
    Gamepad_init();
}

void InputGamepadLib::shutdown()
{
    Gamepad_shutdown();
}

void InputGamepadLib::update()
{
    static unsigned int iterationsToNextPoll = static_cast<int>(define::POLL_ITERATION_INTERVAL);

    iterationsToNextPoll -= 1;
    if (iterationsToNextPoll == 0)
    {
        Gamepad_detectDevices();
        iterationsToNextPoll = static_cast<int>(define::POLL_ITERATION_INTERVAL);
    }

    Gamepad_processEvents();
}

void InputGamepadLib::onButtonDown(Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
{
    Lib::InputGamepadTask::onButtonDown(device, buttonID);
}

void InputGamepadLib::onButtonUp(Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
{
    Lib::InputGamepadTask::onButtonUp(device, buttonID);
}

void InputGamepadLib::onAxisMoved(Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context)
{
    Lib::InputGamepadTask::onAxisMoved(device, axisID, value);
}

void InputGamepadLib::onDeviceAttached(Gamepad_device * device, void * context)
{
    Lib::InputGamepadTask::onDeviceAttached(device);
}

void InputGamepadLib::onDeviceRemoved(Gamepad_device * device, void * context)
{
    Lib::InputGamepadTask::onDeviceRemoved(device);
}