# pragma once

# include "SceneBase.h"

namespace User
{
    class SceneTask : public SceneBase
    {
    public:
        void resize( ) override;
        void update( ) override;
        void draw( ) override;
        void select( ) override;
    public:
        void setup( );
        void shutdown( );
    };
}