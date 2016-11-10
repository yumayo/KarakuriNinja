#pragma once

#include "cinder/app/AppNative.h"

namespace Lib
{
    class Environment
    {
    public:
        static Environment& Geti( );
    public:
        void setup( cinder::app::AppNative *app );
    public:
        cinder::app::AppNative& GetApp( );
    private:
        cinder::app::AppNative *app;
    };
}