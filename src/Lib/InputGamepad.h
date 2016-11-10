#pragma once

#include <memory>
#include <string>

namespace Lib
{
    class InputGamepad
    {
        struct _m_InputKey;
        std::shared_ptr<_m_InputKey> member;
    public:
        InputGamepad(const unsigned int vendorID, const unsigned int productID);
    public:
        bool isPush(const size_t buttonID) const;
        bool isPress(const size_t buttonID) const;
        bool isPull(const size_t buttonID) const;
        float axis(const size_t buttonID) const;
        void end( );
        void flushInput();
    public:
        void onButtonDown(const unsigned int buttonID);
        void onButtonUp(const unsigned int buttonID);
        void onAxisMoved(const unsigned int axisID, const float value);
    public:
        static std::string createHandleID(const unsigned int vendorID, const unsigned int productID);
        void SetHandleID(const std::string& handleID);
        std::string GetHandleID() const;
    };
}