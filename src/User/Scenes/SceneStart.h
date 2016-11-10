# pragma once

# include "SceneTitle.h"

namespace User
{
    SceneBasePtr start( );

    // このスタート関数で一番初めに実行するシーンを return new してください。
    inline SceneBasePtr start( )
    {
        env.setWindowSize( 1280, 720 );
        return new SceneTitle( );
    }
}