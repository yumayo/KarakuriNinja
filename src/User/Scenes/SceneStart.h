# pragma once

# include "SceneBegin.h"
# include "Framework.hpp"

namespace User
{
    SceneBasePtr start( );

    // このスタート関数で一番初めに実行するシーンを return new してください。
    inline SceneBasePtr start( )
    {
        env.setWindowSize( 1280, 720 );
        env.setWindowPos( ( env.getDisplay( )->getSize( ) - env.getWindowSize( ) ) / 2 );

        return new SceneBegin( );
    }
}