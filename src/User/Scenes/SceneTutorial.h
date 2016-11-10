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
    public:
        virtual void draw( ) override;
        virtual void update( ) override;
        virtual void select( ) override;
    public:
        void UpdatePlayer( );
        void UpdateColor( );
        void UpdateScore( );
    private:
        void beginDrawMain( );
        void drawMain( );
        void endDrawMain( );
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
        //=======================================
        // ���[�}�����Ǘ�������̂̃C���X�^���X���B

        cinder::Vec3f cameraEyePosition;
        cinder::CameraPersp camera;
        EnemyManagerRef enemyManager;
        EnemyBulletManagerRef enemyBulletManager;
        FieldManagerRef fieldManager;
        EffectManagerRef effectManager;
        InterfaceRef UI;
        Timer timer;
        cinder::ColorA damageColor;
        //=======================================


        //=======================================
        // ��{���Ǘ�������̂̃C���X�^���X���B

        Player player;
        //=======================================


        //=======================================
        // �傿��񂪊Ǘ�������̂̃C���X�^���X���B

        Special special;
        std::vector<Audio*> mainbgm;
        //=======================================

        //=======================================
        // �`���[�g���A���݂̂ɑ��݂�����́B

        bool isStopEnemy = false;
        //=======================================
    };
}