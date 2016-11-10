# pragma once

# include "cinder/params/Params.h"

namespace User
{
    class GlobalEditor
    {
        GlobalEditor( );
    public:
        static cinder::params::InterfaceGlRef editor;
    };
}