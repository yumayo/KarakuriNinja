#include "SceneCreater.h"

namespace User
{
    SceneBasePtr SceneCreater::sceneBasePtr = nullptr;

    SceneBaseSee SceneCreater::GetScene( )
    {
        return *( sceneBasePtr );
    }

    void SceneCreater::shutdown( )
    {
        delete sceneBasePtr;
    }
}