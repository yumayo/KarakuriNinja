#include"Water.h"

void Water::update()
{
	count++;
	if (count > 180) {
		end = true;
	}
}

void Water::draw()
{
	drawBigWater();
}

void Water::drawBigWater()
{
	const int CUTNUM = 9;
	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth() / 2, app::getWindowHeight() / 2 + 50));
	gl::scale(Vec2f(texsize.x / tex_.getSize().x, texsize.y / (tex_.getSize().y / CUTNUM)));
	cinder::gl::color(cinder::ColorA(1, 1, 1, 0.8));
	int countspeed = 4;
	int index = (count / countspeed) % CUTNUM;
	float start_y = (index % CUTNUM)*(tex_.getSize().y / CUTNUM);
	cinder::gl::draw(tex_,
		cinder::Area(0, start_y, tex_.getSize().x, start_y + int(tex_.getSize().y / CUTNUM)),
		cinder::Rectf(-texsize / 2, texsize / 2));
	gl::popModelView();
}
