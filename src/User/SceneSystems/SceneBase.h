#pragma once

namespace User
{
    class SceneBase;
    using SceneBasePtr = SceneBase*;
    using SceneBaseSee = SceneBase&;

    // �֐����Ă΂�鏇�Ԃ�
    // resize --> update --> draw --> select�̏��ł��B
    class SceneBase
    {
    public:
        virtual ~SceneBase();
    public:
        // ��ʃT�C�Y���ύX���ꂽ���ɌĂяo����܂��B
        // �J�����̃��T�C�Y�Ȃǂ������ōs���܂��傤�B
        virtual void resize();
    public:
        virtual void update();
    public:
        virtual void draw( );
        // �V�[���J�ڐ�p�̊֐��ł��B
        // �����ȊO�ł̃V�[���؂�ւ��͖���`�ɓ���̂ŋC�����܂��傤�B
        virtual void select();
    };

    //==============================
    // �`�R�s�[�p�`
    //==============================
    //# pragma once
    //# include "Scene.hpp"
    //class SceneTy : public SceneBase
    //{
    //public:
    //    SceneTy( );
    //    ~SceneTy( );
    //public:
    //    void resize( );
    //    void update( );
    //    void draw( );
    //    void select( );
    //};
}