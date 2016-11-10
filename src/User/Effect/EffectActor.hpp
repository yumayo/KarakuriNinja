# pragma once

# include "cinder/Camera.h"

namespace User
{
    class EffectActor
    {
    public:
        virtual void Update( ) = 0;
        virtual void Draw( cinder::CameraPersp const& camera ) = 0;
    };
}