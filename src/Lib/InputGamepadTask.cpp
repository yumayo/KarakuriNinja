#include "InputGamepadTask.h"

#include "InputGamepadLib.h"

extern "C"
{
# include "Gamepad.h"
}

#include "InputGamepad.h"

#include "Framework.hpp"

namespace Lib
{
    struct InputGamepadTask::_m_InputGamepadTask
    {
        std::vector<InputGamepad> gamepads;
    };

    std::shared_ptr<InputGamepadTask::_m_InputGamepadTask> InputGamepadTask::member = std::make_shared<InputGamepadTask::_m_InputGamepadTask>();

    void InputGamepadTask::setup()
    {
        ::InputGamepadLib::setup();
    }

    void InputGamepadTask::update()
    {
        ::InputGamepadLib::update();
    }

    void InputGamepadTask::shutdown()
    {
        ::InputGamepadLib::shutdown();
    }

    bool InputGamepadTask::isPush(const std::string& handle, const size_t buttonID)
    {
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == handle)
            {
                return obj.isPush(buttonID);
            }
        }
        return false;
    }

    bool InputGamepadTask::isPress(const std::string& handle, const size_t buttonID)
    {
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == handle)
            {
                return obj.isPress(buttonID);
            }
        }
        return false;
    }

    bool InputGamepadTask::isPull(const std::string& handle, const size_t buttonID)
    {
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == handle)
            {
                return obj.isPull(buttonID);
            }
        }
        return false;
    }

    float InputGamepadTask::axis(const std::string& handle, const size_t buttonID)
    {
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == handle)
            {
                return obj.axis(buttonID);
            }
        }
        return 0.0f;
    }

    bool InputGamepadTask::isPush(const size_t handle, const size_t buttonID)
    {
        if (!member->gamepads.empty())
        {
            return member->gamepads.at(handle).isPush(buttonID);
        }
        return false;
    }

    bool InputGamepadTask::isPress(const size_t handle, const size_t buttonID)
    {
        if (!member->gamepads.empty())
        {
            return member->gamepads.at(handle).isPress(buttonID);
        }
        return false;
    }

    bool InputGamepadTask::isPull(const size_t handle, const size_t buttonID)
    {
        if (!member->gamepads.empty())
        {
            return member->gamepads.at(handle).isPull(buttonID);
        }
        return false;
    }

    float InputGamepadTask::axis(const size_t handle, const size_t buttonID)
    {
        if (!member->gamepads.empty())
        {
            return member->gamepads.at(handle).axis(buttonID);
        }
        return 0.0f;
    }

    void InputGamepadTask::end( )
    {
        flushInput( );
    }

    void InputGamepadTask::flushInput()
    {
        for (auto& obj : member->gamepads) obj.flushInput();
    }

    const std::vector<std::string> InputGamepadTask::GetHandleIDs()
    {
        std::vector<std::string> handleIDs;
        for (auto& obj : member->gamepads)
        {
            handleIDs.push_back(obj.GetHandleID());
        }
        return handleIDs;
    }

    void InputGamepadTask::onButtonDown(Gamepad_device * device, unsigned int buttonID)
    {
        CONSOLE << "handleID[" << InputGamepad::createHandleID(device->vendorID, device->productID) << "] " << "buttonID[" << buttonID << "] " << "down" << std::endl;
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == InputGamepad::createHandleID(device->vendorID, device->productID))
            {
                obj.onButtonDown(buttonID);
                return;
            }
        }
    }

    void InputGamepadTask::onButtonUp(Gamepad_device * device, unsigned int buttonID)
    {
        CONSOLE << "handleID[" << InputGamepad::createHandleID(device->vendorID, device->productID) << "] " << "buttonID[" << buttonID << "] " << "up" << std::endl;
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == InputGamepad::createHandleID(device->vendorID, device->productID))
            {
                obj.onButtonUp(buttonID);
                return;
            }
        }
    }

    void InputGamepadTask::onAxisMoved(Gamepad_device * device, unsigned int axisID, float value)
    {
        CONSOLE << "handleID[" << InputGamepad::createHandleID(device->vendorID, device->productID) << "] " << "axisID[" << axisID << "] " << "value[" << value << "] " << std::endl;
        for (auto& obj : member->gamepads)
        {
            if (obj.GetHandleID() == InputGamepad::createHandleID(device->vendorID, device->productID))
            {
                obj.onAxisMoved(axisID, std::abs(value) < 0.01f ? 0.0f : value);
                return;
            }
        }
    }

    void InputGamepadTask::onDeviceAttached(Gamepad_device * device)
    {
        CONSOLE << "handleID[" << InputGamepad::createHandleID(device->vendorID, device->productID) << "] " << "attached" << std::endl;
        member->gamepads.push_back(InputGamepad(device->vendorID, device->productID));
    }

    void InputGamepadTask::onDeviceRemoved(Gamepad_device * device)
    {
        CONSOLE << "handleID[" << InputGamepad::createHandleID(device->vendorID, device->productID) << "] " << "removed" << std::endl;
        for (auto itr = member->gamepads.cbegin(); itr != member->gamepads.cend();)
        {
            if (itr->GetHandleID() == InputGamepad::createHandleID(device->vendorID, device->productID))
            {
                itr = member->gamepads.erase(itr);
                continue;
            }
            else
            {
                ++itr;
            }
        }
    }
}