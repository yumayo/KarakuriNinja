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
# include "../Interfaces/Talk.h"

//Nomoto's include
# include "../Player/Player.h"

//Hirasawa's include
# include "../Utilitys/Audio.h"
# include "../Special/Special.h"

namespace User
{
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
        void UpdatePlayerBegin( );
        void UpdatePlayerBehavior( );
        void UpdatePlayerEnd( );
        void UpdateColor( );
        void UpdateScore( );
        void UpdateDamageExpression( );
        void UpdateNextStage( );
        void UpdateAllInstans( );
        void UpdateSpecial( );
    private:
        void UpdateDebugTutorialClear( );
        void UpdateTutorialClear( );
    private:
        //=======================================
        // ユーマヨが管理するもののインスタンス化。

        cinder::Vec3f cameraEyePosition;
        cinder::CameraPersp camera;
        EnemyManagerRef enemyManager;
        EnemyBulletManagerRef enemyBulletManager;
        FieldManagerRef fieldManager;
        EffectManagerRef effectManager;
        InterfaceRef UI;
        Timer timer;
        int gameClearFrame;
        cinder::ColorA damageColor;
        TalkRef talk;
        //=======================================


        //=======================================
        // 野本が管理するもののインスタンス化。

        Player player;
        //=======================================


        //=======================================
        // 大ちゃんが管理するもののインスタンス化。

        Special special;
        std::vector<Audio*> mainbgm;
        //=======================================
    };
}