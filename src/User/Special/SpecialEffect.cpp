#include"../Special/SpecialEffect.h"

void SpecialEffect::draw()//‚Æ‚è‚ ‚¦‚¸RECT‚ð•`‰æ‚µ‚Ä‚¨‚«‚Ü‚·
{
	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth()/2.f,app::getWindowHeight()/2));
	gl::rotate(angle_);
	switch (specialtype_)
	{
	case SpecialType::FIRE:
		gl::color(1, 0, 0);
		break;
	case SpecialType::WATER:
		gl::color(0, 0, 1);
		break;
	case SpecialType::TREE:
		gl::color(0, 1, 0);
		break;
	default:
		break;
	}
	gl::drawSolidRect(ci::Rectf(-Vec2f(500, 50), Vec2f(500, 50)));
	gl::color(1, 1, 1);
	gl::popModelView();
}

void SpecialEffect::update()
{
	count_++;
	angle_ += 4.f;
	if (count_ > 120) {
		go_next_ = true;
	}
}
