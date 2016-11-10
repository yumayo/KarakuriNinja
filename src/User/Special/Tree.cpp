#include "Tree.h"

void Tree::update()
{
	count++;
	if (count > 150) {
		end = true;
	}
}

void Tree::draw()
{
	drawBigTree();
}

void Tree::drawBigTree()
{
	const int CUTNUM = 8;
	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth() / 2+450*cos(float(count)/10.f + (2 * M_PI / 3.f)), app::getWindowHeight() / 3 + 50 * sin(float(count) / 10.f+(2*M_PI/3.f))));
	gl::scale(Vec2f(texsize.x / (tex_.getSize().x/CUTNUM), texsize.y / tex_.getSize().y));
	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	int countspeed = 3;
	int index = (count / countspeed) % CUTNUM;
	float start_x = (index % CUTNUM)*(tex_.getSize().x / CUTNUM);
	cinder::gl::draw(tex_,
		cinder::Area(start_x, 0, start_x + int(tex_.getSize().x / CUTNUM),tex_.getSize().y ),
		cinder::Rectf(-texsize / 2, texsize / 2));
	gl::popModelView();
}
