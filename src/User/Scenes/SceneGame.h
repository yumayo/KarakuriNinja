# pragma once

# include "Scene.hpp"

# include "cinder/Camera.h"

//Yumayo's include
# include "../Enemys/EnemyManager.h"
# include "../Enemys/EnemyBulletManager.h"
# include "../Field/FieldManager.h"
# include "../Interfaces/Interface.h"
# include "../Utilitys/Yumayo.h"
#include "../Utilitys/Audio.h"
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
		virtual void select() override;
    public:
        void UpdatePlayer( );
        void UpdateColor( );
        void UpdateScore( );
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

		// ユーマヨが管理するもののインスタンス化。

		EnemyManagerRef enemyManager;
		EnemyBulletManagerRef enemyBulletManager;
        FieldManagerRef fieldManager;
		InterfaceRef UI;
        int gameClearFrame;

        Timer timer;
        bool カメラを揺らす;
        cinder::ColorA damageColor;
        //

        // 野本が管理するもののインスタンス化。

		Player player;
        //


        // 大ちゃんが管理するもののインスタンス化。

        Special special;
		std::vector<Audio> mainbgm;
        gl::Texture handtex[4];
        //
	};
}