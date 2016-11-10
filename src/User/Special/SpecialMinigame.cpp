#include "SpecialMinigame.h"

const int STEPNUM = 3;

void SpecialMinigame::draw()
{
	for (auto &itr : circles) {
		itr.draw();
	}
}

void SpecialMinigame::update()
{
	updateTouch();

	for (auto &itr : circles) {
		itr.update();
	}
	if (isSuccessTouchDoubleCircle()) {
		success_count_++;
	}
	if (success_count_ > 100) {
		success_count_ = 0;
		step_++;
		clearCircles();
		create2Circles();
	}
	if (step_ == 3) {
		clearCircles();
		go_next_ = true;
	}
}

void SpecialMinigame::create2Circles()
{
	
	circles.push_back(Circle(Vec2f(100+step_*150, 100+type_*100), LEFT));//É|ÉXÇÕç°ÇÕìKìñ
	circles.push_back(Circle(Vec2f(500 - (step_ * 150), 500), RIGHT));
}

void SpecialMinigame::updateTouch()
{
	auto touch = inputs.touch();
	auto handles = inputs.GetTouchHandleIDs();

	for (int i = 0;i < circles.size();i++) {
		circles[i].setIsSafe(false);//Ç«Ç¡ÇøÇ‡falseÇ…ÇµÇ∆Ç≠
	}
	for (auto id : handles) {
		if (inputs.isPressTouch(id, touch))
		{
			for (int i = 0;i < circles.size();i++) {
				if (circleCollision(touch.getPos(), circles[i].getPos(), circles[i].getSize()))
					circles[i].setIsSafe(true);//Ç±Ç±Ç≈Ç«Ç¡ÇøÇ‡trueÇ…Ç≈Ç´ÇΩÇÁtouchê¨å˜
			}
		}
	}
}
void SpecialMinigame::clearCircles()
{
	circles.clear();
}
bool SpecialMinigame::circleCollision(const Vec2f pos1, const Vec2f pos2, const float r_size, float level) {
	float length = (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y);
	if (length < ((r_size*r_size)*level)) {
		return true;
	}
	else {
		return false;
	}
}