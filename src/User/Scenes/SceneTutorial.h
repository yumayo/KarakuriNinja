# pragma once

# include "Scene.hpp"

//Yumayo's include
# include "cinder/Camera.h"
# include "../Enemys/EnemyManager.h"
# include "../Enemys/EnemyBulletManager.h"
# include "../Field/FieldManager.h"
# include "../Effect/EffectManager.h"
# include "../Interfaces/Interface.h"
# include "../Utilitys/Yumayo.h"
# include "../Utilitys/CameraData.h"
# include "../Utilitys/TutorialManager.h"

//Nomoto's include
# include "../Player/Player.h"

//Hirasawa's include
# include "../Utilitys/Audio.h"
# include "../Special/Special.h"

namespace User
{
    class Fusuma;

    class SceneTutorial : public SceneBase
    {
    public:
        SceneTutorial( );
        virtual ~SceneTutorial( );
    public:
        virtual void resize( ) override;
        virtual void draw( ) override;
        virtual void update( ) override;
        virtual void select( ) override;
    private:
        void beginDrawMain( );
        void drawMain( );
        void endDrawMain( );
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
        void UpdateDamage( );
        void UpdateCombo( );
        void UpdateNextStage( );
        void UpdateCamera( );
        void UpdateAllInstans( );
    public:
        void UpdateEnemySpawn( );
        void UpdateGuardSucceed( );
    private:
        void UpdateTutorialClear( );
    private:
        //=======================================
        // ユーマヨが管理するもののインスタンス化。
        EnemyManagerRef enemyManager;
        EnemyBulletManagerRef enemyBulletManager;
        FieldManagerRef fieldManager;
        EffectManagerRef effectManager;
        int combo = 0;

        CameraData camera;
        MoveInput moveInput;
        TutorialManager tutorialManager;
        //=======================================


        //=======================================
        // 野本が管理するもののインスタンス化。

        Player player;
        //=======================================


        //=======================================
        // 大ちゃんが管理するもののインスタンス化。

        Audio* mainbgm;
        std::shared_ptr<Fusuma> fusuma;
        //=======================================
    };
}