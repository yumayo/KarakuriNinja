# pragma once

# include "Scene.hpp"

# include "../Utilitys/Yumayo.h"
#include"../Utilitys/Hirasawa.h"
namespace User
{
	class SceneTitle : public SceneBase
	{
	public:
		SceneTitle();
		~SceneTitle();
	public:
		void resize();
		void update();
		void draw();
		void select();
	public:
		void beginDrawUI();
		void drawUI();
		void endDrawUI();
	private:
		// スラッシュでの操作を行いますので、インスタンス化します。
		// Yumayo.h
		MoveInput moveInput;

		// Yumayo.h のフォントを扱うクラスです。
		float endt_;
		Audio* bgm;
		// シーン遷移に使います。
		bool isEnd;
		bool isStartTouch();
		bool isblackout;
		void drawfade();
	};
}