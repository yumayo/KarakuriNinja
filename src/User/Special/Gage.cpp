#include"Gage.h"

void Gage::draw()
{


	static float size = 0;
	size+=0.001;
	size=std::min(size,1.0f);
	gagedraw(texs[GAGE], Vec2f(600, 70), Vec2f(app::getWindowWidth() / 2, app::getWindowHeight() / 1.1f),
		cinder::Area(0, 0, texs[GAGE].getSize().x, texs[GAGE].getSize().y),size);

	//gl::pushModelView();
	//gl::translate(Vec2f(app::getWindowWidth() / 2, app::getWindowHeight() / 1.1f));
	//gl::scale(Vec2f(600, 70));
	//gl::enableAlphaBlending();
	//texs[GAGE].enableAndBind();
	//gl::drawSolidRect(ci::Rectf(Vec2f(-0.5f, -0.5f), Vec2f(0.5f, 0.5f)));
	//texs[GAGE].disable();
	//gl::popModelView();

	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth()/2, app::getWindowHeight() / 1.1f));
	gl::scale(Vec2f(600,70));
	gl::enableAlphaBlending();
	texs[WAKU].enableAndBind();
	gl::drawSolidRect(ci::Rectf(Vec2f(-0.5f,-0.5f), Vec2f(0.5f, 0.5f)));
	texs[WAKU].disable();
	gl::popModelView();


}

void Gage::update()
{


}

void Gage::gagedraw(cinder::gl::Texture const & texture, cinder::Vec2f maxsize, cinder::Vec2f position, cinder::Area area, float size)
{
	cinder::gl::pushModelView();
	cinder::gl::translate(position);
	cinder::gl::pushModelView();
	//ci::gl::rotate(-angle * (180.0f / M_PI) + 90);
	ci::gl::scale(Vec2f(maxsize.x/texture.getSize().x, maxsize.y / texture.getSize().y));
	ci::gl::translate(-texture.getSize().x / 2, -texture.getSize().y / 2);
	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	cinder::gl::draw(texture,
		cinder::Area(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2),
		cinder::Rectf(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2));
	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	texture.unbind();
	cinder::gl::popModelView();
	cinder::gl::popModelView();
}
