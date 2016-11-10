# pragma once

# include "SceneBase.h"

namespace User
{
    template<class Ty>
    using SceneTyPtr = Ty*;

    class SceneCreater
    {
        static SceneBasePtr sceneBasePtr;
    public:
        static SceneBaseSee GetScene( );
    public:
        template <class Ty>
        static void create( SceneTyPtr<Ty> scene );
    public:
        static void shutdown( );
    };

    template<class Ty>
    inline void SceneCreater::create( SceneTyPtr<Ty> scene )
    {
        shutdown( );
        sceneBasePtr = scene;
    }
}