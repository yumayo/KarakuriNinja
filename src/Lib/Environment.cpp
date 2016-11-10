#include "Environment.h"

namespace Lib
{
    Environment & Environment::Geti( )
    {
        static Environment instans;
        return instans;
    }
    void Environment::setup( cinder::app::AppNative *app )
    {
        this->app = app;
    }
    cinder::app::AppNative& Environment::GetApp( )
    {
        return *app;
    }
}