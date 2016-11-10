#include"Gage.h"

void Gage::draw()
{

	gagedraw(*texs[GAGE], Vec2f(600, 70), Vec2f(app::getWindowWidth() / 2, app::getWindowHeight() / 1.1f),
		cinder::Area(0, 0, texs[GAGE]->getSize().x, texs[GAGE]->getSize().y),count_);

	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth()/2, app::getWindowHeight() / 1.1f));
	gl::scale(Vec2f(600,70));
	gl::color(ColorA(1, 1, 1, 1));
	gl::enableAlphaBlending();
	texs[WAKU]->enableAndBind();
	gl::drawSolidRect(ci::Rectf(Vec2f(-0.5f,-0.5f), Vec2f(0.5f, 0.5f)));
	texs[WAKU]->disable();
	gl::popModelView();


}

void Gage::update()
{
	for (int i = 0;i < 2;i++) {
		if (issuccces_[i]) {
			float countspeed = 1.0f / (60.0f*5.0f);
			count_ += countspeed;
			count_ = std::min(count_, 1.0f);
		}
	}
	

}

void Gage::gagedraw(cinder::gl::Texture const & texture, cinder::Vec2f maxsize, cinder::Vec2f position, cinder::Area area, float size)
{
	cinder::gl::pushModelView();
	cinder::gl::translate(position);
	cinder::gl::pushModelView();
	//ci::gl::rotate(-angle * (180.0f / M_PI) + 90);
	ci::gl::scale(Vec2f(maxsize.x/texture.getSize().x, maxsize.y / texture.getSize().y));
	ci::gl::translate(-texture.getSize().x / 2, -texture.getSize().y / 2);
	if ((issuccces_[0]||issuccces_[1])&&(!(count_==1.0f))) {
		gl::color(ColorA(1, 1, 1, 0.7 + 0.3*sin(count_*100.f)));
	}
	else {
		gl::color(ColorA(1, 1, 1, 1));
	}
	cinder::gl::draw(texture,
		cinder::Area(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2),
		cinder::Rectf(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2));
	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	texture.unbind();
	cinder::gl::popModelView();
	cinder::gl::popModelView();
}
