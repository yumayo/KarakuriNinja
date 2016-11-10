#include "Haguruma.h"

using namespace ci;

namespace User
{

	Haguruma::Haguruma()
	{

	}

	Haguruma::Haguruma(cinder::Vec2f pos, cinder::Vec2f scale, float starttime, float endtime, float rotatespeed, HagurumaType type, bool playsound)
	{
		
		pos_ = pos;
		scale_ = scale;
		starttime_ = starttime;
		endtime_ = endtime;
		t_ = 0.0f;
		type_ = type;
		rotatespeed_ = rotatespeed;
		for (int i = 0;i < 6;i++) {
			tex[i] = &GData::FindTexture("Textures/Haguruma/gear" + std::to_string(i) + ".png");
			rotate[i] = 0.0f;
		}
		anglescale_t_ = 0.0f;
		updatetype_ = NOTUPDATE;
		se = &GData::FindAudio("SE/haguruma1.wav");
		playsound_ = playsound;
	}

	void Haguruma::draw()
	{
       
		if (updatetype_ == NOTUPDATE)return;
		drawTexture(Vec2f(-72, -67), 3);
		drawTexture(Vec2f(75, 45), 4);
		drawTexture(Vec2f(43, 92), 5);
		drawTexture(Vec2f(0, 0), 2);
		drawTexture(Vec2f(-155, -10), 0);
		drawTexture(Vec2f(26, 149), 1);
	}

	void Haguruma::update()
	{
		switch (updatetype_)
		{
		case NOTUPDATE:
			//‰½‚à‚µ‚È‚¢‚æ
			break;
		case STARTUPDATE:
			startUpdate();
			break;
		case ENDUPDATE:
			endUpdate();
			break;
		default:
			break;
		}

	
	}

	void Haguruma::drawTexture(cinder::Vec2f trancepos, int num)
	{
		float scalex = Easing::getEasing[Easing::Linear](anglescale_t_,0.f,1.f);
		gl::pushModelView();
		Vec2f trance;
		trance.x = trancepos.x*(scale_.x / 300.f);
		trance.y = trancepos.y*(scale_.y / 300.f);
		gl::translate(pos_);
		if (type_ == LEFTHAGURUMA) {
			gl::scale(Vec2f(-1.f, 1.f));
		}
		else {
			gl::scale(Vec2f(1.f, 1.f));
		}
		gl::translate(trance);
		gl::rotate(rotate[num]);
		Vec2f scale;
		scale.x = (((float(tex[num]->getSize().x))*scale_.x) / 300.f)*scalex;
		scale.y = ((float(tex[num]->getSize().y))*scale_.y) / 300.f;
		gl::color(ColorA(1, 1, 1, 1));
		gl::scale(scale);
		tex[num]->enableAndBind();
		gl::drawSolidRect(Rectf(Vec2f(-0.5f, -0.5f), Vec2f(0.5f, 0.5f)));
		tex[num]->disable();
		gl::popModelView();
	}

	void Haguruma::startUpdate()
	{
		Easing::tCount(anglescale_t_, starttime_);
		angleUpdate();
		if ((!se->IsPlaying())&&playsound_) {
			se->Gain(1.0f);
			se->Play();
			se->Looping(true);
		}
	}

	void Haguruma::endUpdate()
	{
		anglescale_t_ -= 1.f / (60.f*endtime_);
		anglescale_t_ = std::max(0.0f, anglescale_t_);
		angleUpdate();
		se->Gain(anglescale_t_);
		if (anglescale_t_ <= 0.0f) {
			updatetype_ = NOTUPDATE;
			se->Stop();
		}
	}

	void Haguruma::angleUpdate()
	{
		float angletype;
		if (type_ == LEFTHAGURUMA) {
			angletype = 1.f;
		}
		else {
			angletype = -1.f;
		}
		rotate[0] -= 1 * rotatespeed_*angletype;
		rotate[1] -= 1 * rotatespeed_*angletype;
		rotate[2] += 1 * rotatespeed_*angletype;
		rotate[3] += 2 * rotatespeed_*angletype;
		rotate[4] -= 2 * rotatespeed_*angletype;
		rotate[5] += 2 * rotatespeed_*angletype;
	}

}