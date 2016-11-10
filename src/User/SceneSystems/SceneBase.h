#pragma once

namespace User
{
    class SceneBase;
    using SceneBasePtr = SceneBase*;
    using SceneBaseSee = SceneBase&;

    // 関数が呼ばれる順番は
    // resize --> update --> draw --> selectの順です。
    class SceneBase
    {
    public:
        virtual ~SceneBase();
    public:
        // 画面サイズが変更された時に呼び出されます。
        // カメラのリサイズなどをここで行いましょう。
        virtual void resize();
    public:
        virtual void update();
    public:
        virtual void draw( );
        // シーン遷移専用の関数です。
        // ここ以外でのシーン切り替えは未定義に入るので気をつけましょう。
        virtual void select();
    };

    //==============================
    // ～コピー用～
    //==============================
    //# pragma once
    //# include "Scene.hpp"
    //class SceneTy : public SceneBase
    //{
    //public:
    //    SceneTy( );
    //    ~SceneTy( );
    //public:
    //    void resize( );
    //    void update( );
    //    void draw( );
    //    void select( );
    //};
}