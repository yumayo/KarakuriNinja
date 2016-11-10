# pragma once

# include "SceneBase.h"

namespace User
{
    template<class Ty>
    using SceneTyPtr = Ty*;

    class SceneCreater
    {
        static SceneBasePtr sceneBasePtr;
        static SceneBasePtr sceneBasePtrBackUp;
    public:
        static SceneBaseSee GetScene( );
    public:
        template <class Ty>
        static void create( SceneTyPtr<Ty> scene );
        static void setScene( );
    public:
        static void shutdown( );
    };

    template<class Ty>
    inline void SceneCreater::create( SceneTyPtr<Ty> scene )
    {
        sceneBasePtrBackUp = scene;
    }
}