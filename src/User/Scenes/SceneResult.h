# pragma once

# include "Scene.hpp"

namespace User
{
    class SceneResult : public SceneBase
    {
    public:
        SceneResult( );
        ~SceneResult( );
    public:
        void resize( );
        void update( );
        void draw( );
        void select( );
    public:
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
    };
}