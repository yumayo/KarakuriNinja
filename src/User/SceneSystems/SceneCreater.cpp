#include "SceneCreater.h"

namespace User
{
    SceneBasePtr SceneCreater::sceneBasePtr = nullptr;
    SceneBasePtr SceneCreater::sceneBasePtrBackUp = nullptr;

    SceneBaseSee SceneCreater::GetScene( )
    {
        return *( sceneBasePtr );
    }

    void SceneCreater::setScene( )
    {
        if ( sceneBasePtrBackUp == nullptr ) return;

        shutdown( );
        sceneBasePtr = sceneBasePtrBackUp;
        sceneBasePtrBackUp = nullptr;
    }

    void SceneCreater::shutdown( )
    {
        delete sceneBasePtr;
    }
}