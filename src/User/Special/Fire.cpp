#include "Fire.h"

void Fire::update()
{
	pushFire();
	count++;
	if (count > 180) {
		end = true;
	}
}

void Fire::draw()
{
	drawBigFire();
}

void Fire::pushFire()
{

}

void Fire::drawBigFire()
{
	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth()/2,app::getWindowHeight()/2+50));
	gl::scale(Vec2f(texsize.x/ tex_.getSize().x, texsize.y/(tex_.getSize().y/8.f)));
	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	int countspeed = 2;
	int index = (count / countspeed) % 8;
	float start_y = (index % 8)*(tex_.getSize().y / 8.f);
		cinder::gl::draw(tex_,
			cinder::Area(0, start_y,tex_.getSize().x, start_y + int(tex_.getSize().y / 8)),
			cinder::Rectf(-texsize/2,texsize/2));
	gl::popModelView();
}
//void Gage::gagedraw(cinder::gl::Texture const & texture, cinder::Vec2f maxsize, cinder::Vec2f position, cinder::Area area, float size)
//{
//	cinder::gl::pushModelView();
//	cinder::gl::translate(position);
//	cinder::gl::pushModelView();
//	//ci::gl::rotate(-angle * (180.0f / M_PI) + 90);
//	ci::gl::scale(Vec2f(maxsize.x / texture.getSize().x, maxsize.y / texture.getSize().y));
//	ci::gl::translate(-texture.getSize().x / 2, -texture.getSize().y / 2);
//	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
//	cinder::gl::draw(texture,
//		cinder::Area(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2),
//		cinder::Rectf(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2));
//	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
//	texture.unbind();
//	cinder::gl::popModelView();
//	cinder::gl::popModelView();
//}