#include "Deffence.h"

void Deffence::Update(ci::Vec2f rightPos, ci::Vec2f leftPos)
{
	effect = MakeEffect(rightPos, leftPos);
	guardLine.startPos = rightPos;
	guardLine.endPos = leftPos;

	guardLineSegment = MakeLine(guardLine);
	MakeGuardLine();
}

void Deffence::Draw()
{
	ci::gl::pushModelView();
	ci::gl::translate(effect.pos.x, effect.pos.y);
	ci::gl::rotate(-effect.angle * (180.0f / M_PI) - 90);
	if (isActive)
	{
		ci::gl::color(cinder::ColorA(1,1,1,0.9f));
		tex->enableAndBind();
		ci::gl::drawSolidRect(ci::Rectf(ci::Vec2f(-900, -112.5f), ci::Vec2f(900, 112.5f)));
		tex->disable();
		/*ci::gl::color(0,1,0);
		ci::gl::drawSolidRect(ci::Rectf(ci::Vec2f(-effect.size.x / 2, -45), ci::Vec2f(effect.size.x / 2, 45)));*/
		ci::gl::color(1, 1, 1);
	}
	ci::gl::popModelView();

	//ci::gl::pushModelView();
	//ci::gl::color(1, 0, 0);
	//ci::gl::drawSolidCircle(guardLine.startPos, 20);
	//ci::gl::drawSolidCircle(guardLine.endPos, 20);
	//ci::gl::drawLine(guardLine.startPos,guardLine.endPos);
	//ci::gl::color(1, 1, 1);
	//ci::gl::popModelView();
}