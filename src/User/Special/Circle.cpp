#include"Circle.h"
const int safeframe = 2;
void Circle::update()
{
	setRealSafe();
}

void Circle::draw()
{
	if (isrealsafe_) {
		gl::color(ColorA(1, 0, 0));
	}
	else {
		gl::color(ColorA(1, 1, 1));
	}
	//if(isrealsafe_&&(!issafe_))
	//	gl::color(ColorA(0, 1, 0));

	gl::drawSolidCircle(pos_,size_);
	gl::color(ColorA(1, 1, 1));
}

void Circle::setRealSafe()
{
	if (issafe_ == true) {
		isrealsafe_ = true;
		safecount_ = 0;
	}
	else {
		safecount_++;
	}
	if (safecount_ > safeframe) {
		isrealsafe_ = false;
		safecount_ = 0;
	}
}
