#include "Combo.h"

#include "GlobalData.hpp"

namespace User
{
	using namespace cinder;

	Combo::Combo()
		: font(u8"HGçsèëëÃ", 85)
		, backGround(&GData::FindTexture("UI/ReamHammer.png"))
		, effect("Textures/Effect/pipo-btleffect102c.png",
			Vec2f::zero(),
			0.0F,
			Vec2f(480, 480),
			Vec2f(240, 240),
			EffectBase::Mode::LEFTUP)
		, comboNumber(0)
		, maxComboNumber(comboNumber)
	{
		for (int i = 0;i < 10;i++) {
			num1[i] = &GData::FindTexture("Textures/renzan/combo-1-" + std::to_string(i) + ".png");
		}
		for (int i = 1;i < 10;i++) {
			num10[i - 1] = &GData::FindTexture("Textures/renzan/combo-10-" + std::to_string(i) + ".png");
		}
		combo = &GData::FindTexture("Textures/renzan/combo.png");
		t_ = 1.0f;
	}
	void Combo::Update()
	{
		if (comboNumber == 0) return;

		effect.Update();
	}
	void Combo::PlusCombo(int attackSuccessNum)
	{
		if (0 < attackSuccessNum)
		{
			comboNumber = std::min(comboNumber + attackSuccessNum, 99);
			if (maxComboNumber < comboNumber) maxComboNumber = comboNumber;
			t_ = 0.0f;
		}
	}
	void Combo::ResetCombo()
	{
		comboNumber = 0;
	}
	void Combo::Draw(cinder::Vec2f position)
	{
		if (comboNumber != 0)
		{
			gl::pushModelView();
			gl::translate(position);

			effect.Draw();

			//gl::draw(*backGround);
			//font.Draw(std::to_string(comboNumber), Vec2f(60, 95), Color::white());
			drawCombo();
			gl::popModelView();
		}
	}
	void Combo::drawCombo()
	{
		gl::pushModelView();
		gl::translate(Vec2f(80,110));
		gl::color(ColorA(1,1,1,1));

		Vec2f startsize;
		startsize.x = 340;
		startsize.y = startsize.x*(float(combo->getSize().y)/float(combo->getSize().x));
		Vec2f size;
		size.x = Easing::getEasing[Easing::Return](t_, startsize.x, startsize.x / 3.f);
		size.y = Easing::getEasing[Easing::Return](t_, startsize.y, startsize.y / 3.f);
		Easing::tCount(t_,0.2f);

		combo->enableAndBind();
		gl::drawSolidRect(Rectf(-size/2,size/2));
		combo->disable();

		if (comboNumber >= 10) {
			num10[(comboNumber / 10) - 1]->enableAndBind();
			gl::drawSolidRect(Rectf(-size / 2, size / 2));
			num10[(comboNumber / 10) - 1]->disable();
		}

		num1[comboNumber % 10]->enableAndBind();
		gl::drawSolidRect(Rectf(-size / 2, size / 2));
		num1[comboNumber % 10]->disable();
		gl::popModelView();
	}
}