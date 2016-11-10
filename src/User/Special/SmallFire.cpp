#include"SmallFire.h"

void SmallFire::update()
{
	count++;
}

void SmallFire::draw()
{
	//Vec2f size = Vec2f(100, 100);
	//gl::pushModelView();
	//gl::translate(Vec2f(app::getWindowWidth() / 2, app::getWindowHeight() / 2 + 50));
	//gl::scale(Vec2f(size.x / (image.getSize().x/8.f),size.y / image.getSize().y));
	//cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	//int countspeed = 2;
	//int index = (count / countspeed) % 8;
	//float start_x = (index % 8)*(image.getSize().x / 8.f);
	//cinder::gl::draw(image,
	//	cinder::Area(0, start_y, bigfiretex_.getSize().x, start_y + int(bigfiretex_.getSize().y / 8)),
	//	cinder::Rectf(-bigfiresize / 2, bigfiresize / 2));
	//gl::popModelView();
}
