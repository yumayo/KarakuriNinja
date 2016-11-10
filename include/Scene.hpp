# pragma once

// ���̃w�b�_�[�t�@�C�����C���N���[�h���邱�ƂŁA
// �V�[���x�[�X�̃C���N���[�h�ƁA�V�[���쐬�̊֐����g�����Ƃ��o���܂��B

# include "../src/User/SceneSystems/SceneBase.h"
# include "../src/User/SceneSystems/SceneCreater.h"

namespace User
{
    // ���̊֐��ŃV�[�����쐬�ł��܂��B
    // ���������A�V�[�����쐬����̂�select�֐����ōs���Ă��������B
    template <class Ty>
    void create( SceneTyPtr<Ty> scene );

    template<class Ty>
    inline void create( SceneTyPtr<Ty> scene )
    {
        SceneCreater::create( scene );
    }
}