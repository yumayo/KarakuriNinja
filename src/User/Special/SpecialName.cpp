#include "SpecialName.h"

void SpecialName::update()
{
	Easing::tCount(t_,0.3f);
	pos_.x = Easing::getEasing[Easing::QuintIn](t_, app::getWindowWidth() / 2, endpos_.x);
	pos_.y = Easing::getEasing[Easing::QuintIn](t_, 250, endpos_.y);
	size_.x = Easing::getEasing[Easing::QuintIn](t_, 500, endsize_.x);
	size_.y = Easing::getEasing[Easing::QuintIn](t_, 500,endsize_.y);
}

void SpecialName::draw()
{
	gl::pushModelView();
	gl::enableAlphaBlending();
	gl::translate(pos_);
	gl::scale(size_);
	gl::color(color_);
	tex_.enableAndBind();
	gl::drawSolidRect(ci::Rectf(Vec2f(-0.5f, -0.5f), Vec2f(0.5f, 0.5f)));
	tex_.disable();
	gl::popModelView();
}
