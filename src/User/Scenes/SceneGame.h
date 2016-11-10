# pragma once

# include "Scene.hpp"

# include "cinder/Camera.h"

//Yumayo's include
# include "../Enemys/EnemyController.h"
# include "../Enemys/EnemyBulletManager.h"
# include "../Interfaces/Interface.h"
# include "../Field/Field.h"

//Nomoto's include
# include "../Attack/AttackFactory.h"
# include "../SlashEffect/SlashEffect.h"
# include "../Player/Player.h"
# include "../Camera/Camera.h"

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
		cinder::CameraPersp cameraPersp;

		// ユーマヨが管理するもののインスタンス化。
		EnemyControllerRef enemyController;
		EnemyBulletManagerRef enemyBulletManager;
		Interface UI;
        FieldRef field;

		Player player;
	};
}