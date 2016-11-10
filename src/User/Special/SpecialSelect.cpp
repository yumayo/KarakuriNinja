#include"SpecialSelect.h"



void SpecialSelect::draw()
{
	gl::pushModelView();
	drawYazirushi();
	drawSubInfo();
	drawIcon();
	drawbeginSpecial();
	for (int i = 0;i<shurikens.size();i++) {
		shurikens[i].draw(Vec2f(0, beginposy));
	}
	drawKuriko();
	gl::popModelView();
}

void SpecialSelect::update(bool isstop)
{
	updateIcon();
	updateSubInfo();
	chooseSpecial();
	updateShuriken();
	beginposy = env.getWindowHeight() / 2.f;
	cutinsizey = env.getWindowHeight() / 2.f;
}

void SpecialSelect::drawIcon()
{
	if (!shurikenEnd())return;
	for (int i = 0;i < icons.size();i++) {
		gl::pushModelView();
		gl::translate(icons[i].pos);
		gl::rotate(Vec3f(0.f,icons[i].angle,0));
		gl::color(ColorA(1,1,1,1));
		icontex[i]->enableAndBind();
		gl::drawSolidRect(ci::Rectf(-icons[i].size / 2.f, icons[i].size / 2.f));
		icontex[i]->disable();
		gl::popModelView();
	}
}

void SpecialSelect::updateIcon()
{
	if (!shurikenEnd())return;
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
			open_se[0]->Play();
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
			Vec2f startpos = Vec2f(app::getWindowWidth() / 2 , app::getWindowHeight() / 2);
			icons[int(specialtype_)].pos.y = EasingBackIn(last_t_, startpos.y, startpos.y - 600);
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

	auto hand = inputzkoo.hand();
	for (auto& i : inputzkoo.GetHandleIDs())
	{
		if (inputzkoo.isPush(i, hand))
		{
			for (int i = 0;i < icons.size();i++) {
				if (circleCollision(hand.Position(), icons[i].pos, icons[i].size.x / 2.f)) {
					ischoosed_ = true;
					specialtype_ = (SpecialType)(i);
					isshifteasing_ = true;
					icons[i].shiftpos = Vec2f(app::getWindowWidth() / 2, icons[i].endpos.y);
					return;
				}
			}
		}
	}
}
void SpecialSelect::drawbeginSpecial()
{
	if (shurikenEnd())return;
	Vec2f size = Vec2f(env.getWindowWidth(), cutinsizey);
	Vec2f pos = Vec2f(0, beginposy - size.y / 2.f);
	int countspeed = 3;
	Vec2f cutsize = Vec2f(backanimation->getWidth(), backanimation->getHeight() / 5.f);
	int index = (frame / countspeed) % 5;
	DrawCutTexture(backanimation, pos, size, Vec2f(0, index*cutsize.y), cutsize);

	frame++;
}
void SpecialSelect::drawKuriko()
{
	if (shurikenEnd())return;
	if (awake_t_ >= 1.0f) {
		Easing::tCount(delay_t_,0.5f);
	}
    Vec2f size = Vec2f( cutinsizey, cutinsizey )*1.0f;
	if (delay_t_ < 1.0f) {
		
		Vec2f startpos = Vec2f(-size.x*2.f, beginposy - size.y / 2.f);
		Vec2f endpos = Vec2f(env.getWindowWidth()/2-size.x/2, beginposy - size.y / 2.f);
		Vec2f pos;
		pos.x = Easing::getEasing[Easing::QuartOut](awake_t_, startpos.x, endpos.x);
		pos.y = Easing::getEasing[Easing::ExpoIn](awake_t_, startpos.y, endpos.y);
		Vec2f cutsize = kuriko->getSize();
		DrawCutTexture(kuriko, pos, size, Vec2f(0, 0), cutsize);
		Easing::tCount(awake_t_, 0.4f);
	}
	else {
		Vec2f startpos = Vec2f(env.getWindowWidth()/2 - size.x / 2, beginposy - size.y / 2.f);
		Vec2f endpos = Vec2f(env.getWindowWidth() + size.x*2.f, beginposy - size.y / 2.f);
		Vec2f pos;
		pos.x = Easing::getEasing[Easing::CubicIn](awake2_t_, startpos.x, endpos.x);
		pos.y = Easing::getEasing[Easing::ExpoIn](awake2_t_, startpos.y, endpos.y);
		Vec2f cutsize = kuriko->getSize();
		DrawCutTexture(kuriko, pos, size, Vec2f(0, 0), cutsize);
		Easing::tCount(awake2_t_, 0.4f);
	}

}
void SpecialSelect::pushShuriken()
{
	float size = cutinsizey / 2.f;
	gl::Texture* tex = &GData::FindTexture("Textures/Title/titleguard.png");
	for (int i = 0;i < 10;i++) {
		shurikens.push_back(SpecialShuriken(Vec2f(-400-(i*450),randFloat(-size/2,size/2)),Vec2f(size,size),tex));
	}

}
void SpecialSelect::updateShuriken()
{
	for (int i = 0;i < shurikens.size();i++) {
		shurikens[i].update(Vec2f(75, 0));
	}
}
void SpecialSelect::drawYazirushi()
{
	if (!isCanChoose())return;
	for (int i = 0;i < 3;i++) {
		Vec2f size;
		size.x = env.getWindowWidth() / 10.f;
		size.y = 2*size.x*(float(yazirushi->getHeight()) / float(yazirushi->getWidth()));
		Vec2f pos = Vec2f(app::getWindowWidth() / 2 + float((i - 1)*450.f), size.x*1.2f+30*(0.5+0.5*cos(yazirushirotate_)));
		gl::pushModelView();
		gl::translate(pos);
		gl::rotate(90);
		gl::color(ColorA(1,1,1,1));
		yazirushi->enableAndBind();
		gl::drawSolidRect(Rectf(-size / 2, size/2));
		yazirushi->disable();
		gl::popModelView();
	}
	yazirushirotate_ += 0.1f;
}
void SpecialSelect::drawSubInfo()
{
	for (int i = 0;i < 3;i++) {
		gl::pushModelView();
		gl::translate(subpos[i]);
		gl::color(ColorA(1,1,1,1));
		Vec2f size=Vec2f(500,300);
		size.x = 1.3*icons[i].size.x;
		size.y = size.x*(float(subinfo[i]->getHeight())/ float(subinfo[i]->getWidth()));
		subinfo[i]->enableAndBind();
		gl::drawSolidRect(Rectf(-size/2,size/2));
		subinfo[i]->disable();
		gl::popModelView();
	}
}
void SpecialSelect::updateSubInfo()
{
	if (!shurikenEnd())return;

	for (int i = 0;i <3;i++) {
		subpos[i].x = app::getWindowWidth() / 2 + float((i - 1)*450.f);
		subpos[i].y = EasingBackOut(start_t, env.getWindowHeight()+500, env.getWindowHeight()*0.8f);
		subangle[i] = EasingQuintIn(icons[i].angle_t, 5.0f, 0);

	}
	if (isshifteasing_) {
		for (int i = 0;i < 3;i++) {
			subpos[i].x = app::getWindowWidth() / 2 + float((i - 1)*450.f);
			subpos[i].y = EasingCubicIn(end_t_, env.getWindowHeight()*0.8f, env.getWindowHeight() + 500);
		}
	}
}
void SpecialSelect::DrawCutTexture(cinder::gl::Texture * texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize)
{
	gl::pushModelView();
	gl::color(ColorA(1,1,1,1));
	gl::translate(-offset + position);
	gl::draw(*texture, Area(offset, offset + cutSize), Area(offset, offset + size));
	gl::popModelView();
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