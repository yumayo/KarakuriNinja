# pragma once

# include "Scene.hpp"

# include "../Utilitys/Yumayo.h"
# include "../Utilitys/Audio.h"

namespace User
{
    class SceneResult : public SceneBase
    {
    public:
        SceneResult( int score );
        ~SceneResult( );
    public:
        void resize( );
        void update( );
        void draw( );
        void select( );
    public:
        void UpdateLogoAlpha( );
    public:
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
        int score;
        Fonts font;
        Audio* audio;
        // スラッシュでの操作を行いますので、インスタンス化します。
        // Yumayo.h
        SlashInput slashInput;
        // シーン遷移に使います。
        bool isEnd;
        // スタートボタンの変数。
        float logoAlpha;
        float logoAlphaSpeed;
        cinder::gl::Texture* texture;
    };
}