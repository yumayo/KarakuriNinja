# pragma once

// このヘッダーファイルをインクルードすることで、
// シーンベースのインクルードと、シーン作成の関数を使うことが出来ます。

# include "../src/User/SceneSystems/SceneBase.h"
# include "../src/User/SceneSystems/SceneCreater.h"

namespace User
{
    // この関数でシーンを作成できます。
    // ※ただし、シーンを作成するのはselect関数内で行ってください。
    template <class Ty>
    void create( SceneTyPtr<Ty> scene );

    template<class Ty>
    inline void create( SceneTyPtr<Ty> scene )
    {
        SceneCreater::create( scene );
    }
}