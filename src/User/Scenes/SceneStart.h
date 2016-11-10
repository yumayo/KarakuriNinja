# pragma once

# include "SceneBegin.h"
# include "Framework.hpp"

namespace User
{
    SceneBasePtr start( );

    // ���̃X�^�[�g�֐��ň�ԏ��߂Ɏ��s����V�[���� return new ���Ă��������B
    inline SceneBasePtr start( )
    {
        env.setWindowSize( 1280, 720 );
        env.setWindowPos( ( env.getDisplay( )->getSize( ) - env.getWindowSize( ) ) / 2 );

        return new SceneBegin( );
    }
}