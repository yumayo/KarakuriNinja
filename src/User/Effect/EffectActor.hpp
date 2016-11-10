# pragma once

namespace User
{
    class EffectActor
    {
    public:
        virtual void Update( ) = 0;
        virtual void Draw( ) = 0;
    };
}