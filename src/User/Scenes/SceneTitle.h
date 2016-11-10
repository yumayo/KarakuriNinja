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
		// �X���b�V���ł̑�����s���܂��̂ŁA�C���X�^���X�����܂��B
		// Yumayo.h
		MoveInput moveInput;

		// Yumayo.h �̃t�H���g�������N���X�ł��B
		float endt_;
		Audio* bgm;
		// �V�[���J�ڂɎg���܂��B
		bool isEnd;
		bool isStartTouch();
		bool isblackout;
		void drawfade();
	};
}