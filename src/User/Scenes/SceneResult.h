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
        // �X���b�V���ł̑�����s���܂��̂ŁA�C���X�^���X�����܂��B
        // Yumayo.h
        SlashInput slashInput;
        // �V�[���J�ڂɎg���܂��B
        bool isEnd;
        // �X�^�[�g�{�^���̕ϐ��B
        float logoAlpha;
        float logoAlphaSpeed;
        cinder::gl::Texture* texture;
    };
}