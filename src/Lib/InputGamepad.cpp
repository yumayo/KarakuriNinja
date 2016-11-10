#include "InputGamepad.h"

#include <set>
#include <vector>
#include <sstream>

namespace Lib
{
    struct InputGamepad::_m_InputKey
    {
        std::string handleID;
        std::set<size_t> push;
        std::set<size_t> press;
        std::set<size_t> pull;
        std::vector<std::pair<size_t, float>> axis;
    };

    InputGamepad::InputGamepad(const unsigned int vendorID, const unsigned int productID)
    {
        member = std::make_shared<InputGamepad::_m_InputKey>();
        SetHandleID(createHandleID(vendorID, productID));
    }

    bool InputGamepad::isPush(const size_t buttonID) const
    {
        return member->push.find(buttonID) != member->push.cend();
    }

    bool InputGamepad::isPress(const size_t buttonID) const
    {
        return member->press.find(buttonID) != member->press.cend();
    }

    bool InputGamepad::isPull(const size_t buttonID) const
    {
        return member->pull.find(buttonID) != member->pull.cend();
    }

    float InputGamepad::axis(const size_t buttonID) const
    {
        for (auto& obj : member->axis)
        {
            if (obj.first == buttonID) return obj.second;
        }
        return 0.0f;
    }

    void InputGamepad::end( )
    {
        flushInput( );
    }

    void InputGamepad::flushInput()
    {
        member->push.clear();
        member->pull.clear();
    }

    void InputGamepad::onButtonDown(const unsigned int buttonID)
    {
        member->push.insert(buttonID);
        member->press.insert(buttonID);
    }

    void InputGamepad::onButtonUp(const unsigned int buttonID)
    {
        member->pull.insert(buttonID);

        member->press.erase(buttonID);
    }

    void InputGamepad::onAxisMoved(const unsigned int axisID, const float value)
    {
        for (auto& obj : member->axis)
        {
            if (obj.first == axisID)
            {
                obj.second = value;
                return;
            }
        }
        member->axis.push_back(std::pair<size_t, float>(axisID, value));
    }

    std::string InputGamepad::createHandleID(const unsigned int vendorID, const unsigned int productID)
    {
        std::stringstream stream;
        stream << vendorID << productID;
        return stream.str();
    }

    void InputGamepad::SetHandleID(const std::string& handleID)
    {
        member->handleID = handleID;
    }

    std::string InputGamepad::GetHandleID() const
    {
        return member->handleID;
    }
}