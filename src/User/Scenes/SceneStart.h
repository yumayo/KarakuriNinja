# pragma once

# include "SceneTitle.h"

namespace User
{
    SceneBasePtr start( );

    // ���̃X�^�[�g�֐��ň�ԏ��߂Ɏ��s����V�[���� return new ���Ă��������B
    inline SceneBasePtr start( )
    {
        env.setWindowSize( 1280, 720 );
        return new SceneTitle( );
    }
}