# pragma once

# include "Transform.hpp"

namespace User
{
    class Actor
    {
    public:
        Actor( ) { }
        virtual ~Actor( ) { }
    public:
        virtual void updateBegin( ) { }
        virtual void update( ) { }
        virtual void updateCollision( ) { }
        virtual void updateEnd( ) { }
        virtual void drawBegin( ) { }
        virtual void draw( ) { }
        virtual void drawEnd( ) { }
    };
}