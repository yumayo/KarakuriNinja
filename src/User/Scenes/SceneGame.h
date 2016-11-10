# pragma once

# include "Scene.hpp"

# include "cinder/Camera.h"

//Yumayo's include
# include "../Enemys/EnemyManager.h"
# include "../Enemys/EnemyBulletManager.h"
# include "../Field/FieldManager.h"
# include "../Interfaces/Interface.h"

//Nomoto's include
# include "../Player/Player.h"

//Hirasawa's include
#include "../Special/Special.h"

namespace User
{
	class SceneGame : public SceneBase
	{
	public:
		SceneGame();
		virtual ~SceneGame();
	public:
		virtual void resize() override;
	public:
		virtual void draw() override;
		virtual void update() override;
		void UpdatePlayer();
	public:
		virtual void select() override;
	private:
		void beginDrawMain();
		void drawMain();
		void endDrawMain();
		void beginDrawUI();
		void drawUI();
		void endDrawUI();
	private:
        cinder::Vec3f cameraEyePosition;
		cinder::CameraPersp camera;

		// ���[�}�����Ǘ�������̂̃C���X�^���X���B

		EnemyManagerRef enemyManager;
		EnemyBulletManagerRef enemyBulletManager;
        FieldManagerRef fieldManager;
		InterfaceRef UI;
        int gameClearFrame;
        //

        // ��{���Ǘ�������̂̃C���X�^���X���B

		Player player;
        //

        // �傿��񂪊Ǘ�������̂̃C���X�^���X���B

        Special special;
        //
	};
}