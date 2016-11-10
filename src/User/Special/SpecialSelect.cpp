#include"SpecialSelect.h"

void SpecialSelect::draw()
{
	gl::pushModelView();

	drawIcon();

	gl::popModelView();
}

void SpecialSelect::update()
{
	updateIcon();
	chooseSpecial();
}

void SpecialSelect::drawIcon()
{
	for (int i = 0;i < icons.size();i++) {
		gl::pushModelView();
		gl::translate(icons[i].pos);
		gl::rotate(Vec3f(0.f,icons[i].angle,0));
		icontex[i].enableAndBind();
		gl::drawSolidRect(ci::Rectf(-icons[i].size / 2.f, icons[i].size / 2.f));
		icontex[i].disable();
		gl::popModelView();
	}
}

void SpecialSelect::updateIcon()
{
	start_t += 1.0f / (60.0f*1.0f);
	start_t = std::min(1.0f, start_t);
	const float speed = 0.5f;
	const float changeratio = 0.5f;
	for (int i = 0;i < icons.size();i++) {
		icons[i].pos.y = EasingBackOut(start_t, icons[i].startpos.y, icons[i].endpos.y);
		if (start_t >= 1.0f) {
			if (i == 0) {
				icons[i].angle_t += 1.0f / (60.f*speed);
			}
			else {
				if (icons[i - 1].angle_t > changeratio)
					icons[i].angle_t += 1.0f / (60.f*speed);
			}
		}
		icons[i].angle_t = std::min(1.0f, icons[i].angle_t);
		if ((!icons[i].opened) && icons[i].angle_t >= 1.0f) {
			icons[i].opened = true;
			open_se[0].Play();
		}
		icons[i].angle = EasingQuintIn(icons[i].angle_t, icons[i].startangle, 0);

	}
	if (isshifteasing_) {
		for (int i = 0;i < icons.size();i++) {
			icons[i].pos.x = EasingCubicIn(end_t_, icons[i].endpos.x, icons[i].shiftpos.x);
			icons[i].pos.y = EasingCubicIn(end_t_, icons[i].endpos.y, icons[i].shiftpos.y);
		}
		end_t_ += 1.0f / (60.0f*0.5f);
		end_t_ = std::min(end_t_, 1.0f);
		if (end_t_ >= 1.0f) {
			last_t_ += 1.0f / (60.0f*1.0f);
			last_t_ = std::min(last_t_, 1.0f);
			Vec2f startpos = Vec2f(app::getWindowWidth() / 2 , app::getWindowHeight() / 2-100);
			icons[int(specialtype_)].pos.y= EasingBackIn(last_t_, startpos.y,startpos.y-600);
			if (last_t_ >= 1.0f) {
				go_next_ = true;
			}
		}
	}

}

void SpecialSelect::chooseSpecial()//とりあえずタッチのプッシュでやっておきました
{
	if (isCanChoose() == false)
		return;

	auto touch = inputs.touch();
	auto handles = inputs.GetTouchHandleIDs();
	for (auto id : handles) {
		if (inputs.isPushTouch(id, touch))
		{
			for (int i = 0;i < icons.size();i++) {
				if (circleCollision(touch.getPos(), icons[i].pos, icons[i].size.x / 2.f)) {
					ischoosed_ = true;
					specialtype_ = (SpecialType)(i);
					isshifteasing_ = true;
					icons[i].shiftpos = Vec2f(app::getWindowWidth()/2, icons[i].endpos.y);
					return;
				}
			}
		}
	}
}
bool SpecialSelect::circleCollision(const Vec2f pos1, const Vec2f pos2, const float r_size, float level) {
	float length = (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y);
	if (length < ((r_size*r_size)*level)) {
		return true;
	}
	else {
		return false;
	}
}