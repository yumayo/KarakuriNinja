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
        // �X���b�V���ł̑�����s���܂��̂ŁA�C���X�^���X�����܂��B
        // Yumayo.h
        SlashInput slashInput;

        // Yumayo.h �̃t�H���g�������N���X�ł��B
        Fonts font;

        // �ȒP�ȃe�N�X�`���}�l�[�W���[�B
        Izanami::Holder<ci::gl::Texture> textures;

        // �X�^�[�g�{�^���̕ϐ��B
        float logoAlpha;
        float logoAlphaSpeed;
        cinder::Vec2f startButtonPosition;

        // �V�[���J�ڂɎg���܂��B
        bool isEnd;
    };
}