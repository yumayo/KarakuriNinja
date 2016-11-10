# pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"

#include "../Utilitys/Yumayo.h"
#include "GlobalData.hpp"
#include"../Utilitys/Hirasawa.h"

#include "../Effect/EffectBase.h"

// êØñºí«â¡ï™
// 8/30 .h ComboÉNÉâÉX
// 8/30 .cpp Combo.Draw(...)
// 8/30 .cpp Combo.Update(...)

namespace User
{
	class Combo
	{
		cinder::gl::Texture* backGround;
		EffectBase effect;
		Fonts font;
		int comboNumber;
		int maxComboNumber;
		int comboFrame;
	public:
		Combo();
        int ComboNumber( ) { return comboNumber; }
		int MaxComboNumber() { return maxComboNumber; }
		void Update(bool isSuccess);
		void Draw(cinder::Vec2f position);
	};

	class Interface
	{
	public:
		Interface();
		void draw(float APNormalized, float HPNormalized, bool ismpmax = false, int specialsubtime = 3);
		void update(bool isAttackSuccess);
		void AddScore(int score) { scorePoint += score; }
		int Score() { return scorePoint; }
        int ComboNumber( ) { return combo.ComboNumber( ); }
		int MaxComboNumber() { return combo.MaxComboNumber(); }
	private:
		cinder::gl::Texture* APEdge;
		cinder::gl::Texture* HPEdge;
		cinder::gl::Texture *score;

		cinder::gl::Texture* APEdgeBase;
		cinder::gl::Texture* APEdgeBasemax;
		cinder::gl::Texture* HPEdgeBase;
		cinder::gl::Texture* HPEdgeBasedamage;

		cinder::gl::Texture* specialeffect;
		cinder::gl::Texture* makimono;
		Fonts font;
		Fonts touchfont;
		int scorePoint;

		Combo combo;
	private:
		void textureDraw(cinder::gl::Texture const& texture, cinder::Vec2f position = cinder::Vec2f::zero());
		void textureDraw(cinder::gl::Texture const& texture, cinder::Vec2f position, cinder::Area area, float value);
		void maxbaseDraw(cinder::gl::Texture const& texture, cinder::Vec2f position, cinder::Area area, float value);
		void drawSpecialEffect(int specialsubtime);
		void drawchargeEffect(bool ismpmax);
		void drawInfo(int specialsubtime);
		void drawchargeInfo(bool _ismpmax);
		int count;
		int maxcount;
		float easingnormal;
		float beginnormal;
		float savenormal;
		float nomarl_t;
		float delay_t;
	};

	using InterfaceRef = std::shared_ptr<Interface>;
}