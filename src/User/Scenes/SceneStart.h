# pragma once

# include "SceneTitle.h"

namespace User
{
    SceneBasePtr start( );

    // ���̃X�^�[�g�֐��ň�ԏ��߂Ɏ��s����V�[���� return new ���Ă��������B
    inline SceneBasePtr start( )
    {
        return new SceneTitle( );
    }
}