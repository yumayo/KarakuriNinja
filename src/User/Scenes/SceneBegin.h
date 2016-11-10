# pragma once

# include "Scene.hpp"

# include "../Utilitys/Yumayo.h"

# include "cinder/gl/Texture.h"

namespace User
{
    class SceneBegin : public SceneBase
    {
    public:
        SceneBegin( );
        ~SceneBegin( );
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
        bool isDraw = false;
        bool isLoad = false;
        cinder::gl::Texture nowLoading;
        cinder::gl::Texture loadBar;
        cinder::gl::Texture kuriko;
        Timer timer;
        Fonts font;
    };
}