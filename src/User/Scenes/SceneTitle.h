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
		void UpdateLogoAlpha();
	public:
		void beginDrawUI();
		void drawUI();
		void endDrawUI();
	private:
		// �X���b�V���ł̑�����s���܂��̂ŁA�C���X�^���X�����܂��B
		// Yumayo.h
		SlashInput slashInput;

		// Yumayo.h �̃t�H���g�������N���X�ł��B
		Fonts font;
		float endt_;
		// �X�^�[�g�{�^���̕ϐ��B
		float logoAlpha;
		float logoAlphaSpeed;
		cinder::Vec2f startButtonPosition;
		Audio* bgm;
		// �V�[���J�ڂɎg���܂��B
		bool isEnd;
		void drawfade();
	};
}