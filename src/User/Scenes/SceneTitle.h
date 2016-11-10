# pragma once

# include "Scene.hpp"
# include "../Assets/Assets.h"

# include "../Utilitys/Yumayo.h"

namespace User
{
    class SceneTitle : public SceneBase
    {
    public:
        SceneTitle( );
        ~SceneTitle( );
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
        // スラッシュでの操作を行いますので、インスタンス化します。
        // Yumayo.h
        SlashInput slashInput;

        // Yumayo.h のフォントを扱うクラスです。
        Fonts font;

        // 簡単なテクスチャマネージャー。
        Izanami::Holder<ci::gl::Texture> textures;

        // スタートボタンの変数。
        float logoAlpha;
        float logoAlphaSpeed;
        cinder::Vec2f startButtonPosition;

        // シーン遷移に使います。
        bool isEnd;
    };
}