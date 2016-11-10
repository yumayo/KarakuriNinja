#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"../Utilitys/Audio.h"
#include"../Utilitys/Yumayo.h"

namespace User
{
	class Ranking {
	public:
		Ranking();
		void update(int score, int maxcombo, int nowhp,bool iscalculate);
		void draw();
	private:
		Fonts font;
		cinder::Vec2f pos_;
		cinder::Vec2f size_;
		std::vector<int>player_scores;
		cinder::gl::Texture* num10[9];
		cinder::gl::Texture* num1[10];
		cinder::gl::Texture* rank;
		int score_;
		int ranking_;
		int frame;
		int rankbuf;
		int drawrank;
		int saverank;
		float t;
		float endt_;
		void roadtext();
		int calcurateRanking(int endscore);
		int calcurateScore(int score, int maxcombo, int nowhp);
	};
}