#include "SpecialMinigame.h"

const int STEPNUM = 1;

void SpecialMinigame::draw()
{
	drawBackground();

	drawBackCircles();
	drawBackninja();
	drawInfo();
	
	if(startend())
	gage_.draw();
	for (auto &itr : circles) {
		itr.draw();
	}
	for (auto &itr : inobject) {
		itr.draw();
	}
	if (endflash) {
		gl::translate(Vec2f(app::getWindowWidth()/2,app::getWindowHeight()/2));
		gl::color(ColorA(1,1,1,end_al_));
		gl::drawSolidRect(ci::Rectf(-app::getWindowSize()/2, app::getWindowSize() / 2));
		gl::color(ColorA(1, 1, 1, 1));
	}
}

void SpecialMinigame::update(bool isstop)
{
	updateTouch();
	if(!startend())
	startBackgroundEasing();
    infoUpdate();
	if (info_t_ >= 1.0f && (!ispush_back_circles_)) {
		for (int i = 0;i < 10;i++) {
			BackCircle backcircle;
			backcircle.pos = Vec2f(i*app::getWindowWidth()/10,randFloat(0,app::getWindowHeight()));
			backcircle.size = Vec2f(70, 25);
			backcircles.push_back(backcircle);
		}
		create2Circles();
		ispush_back_circles_ = true;
	}
	for (auto &itr : circles) {
		itr.update();
	}
	for (int i = 0;i < backcircles.size();i++) {
		backcircles[i].pos.x += 20;
		if (backcircles[i].pos.x>app::getWindowWidth() + 100) {
			backcircles[i].pos.x = -100;
			backcircles[i].pos.y = randFloat(0, app::getWindowHeight());
		}
	}
	for (auto &itr : inobject) {
		itr.update();
	}
	if (circles.size() == 2) {
		if (circles[0].getMoveStart() && circles[1].getMoveStart()) {
			gage_.setisCount(circles[0].getIsRealSafe(), circles[1].getIsRealSafe());
			if (circles[0].getIsRealSafe() || circles[1].getIsRealSafe()) {
				charge_se_[0]->Looping(true);
				if(!charge_se_[0]->IsPlaying())
				charge_se_[0]->Play();
			}
			else {
				charge_se_[0]->Stop();
			}
		}
		if (circles[0].getClear() && circles[1].getClear()) {
			inobject.push_back(InObject(getInObject(),inpush));
			gage_.setisCount(false,false);
			clearCircles();
		}
	}
	else
	{
		charge_se_[0]->Stop();
	}
	if (inobject.size() == 1) {
		if (inobject[0].Mydelete() == true) {
			inobject.clear();
			step_++;
			if (step_ != STEPNUM)
				create2Circles();
		}
	}
	gage_.update();
    if (step_ == STEPNUM) {
	endflash = true;
		//clearCircles();
		//go_next_ = true;
	}
    if (endflash) {
		Easing::tCount(endt_,0.7f);
		end_al_ = Easing::getEasing[Easing::EasingType::CircInOut](endt_, 0.f, 1.f);
		if (endt_ >= 1) {
			go_next_ = true;
		}
    }
}

void SpecialMinigame::create2Circles()
{
	RouteType _type;
	switch (type_)
	{
	case SpecialType::FIRE:
		setFireRoute(_type);
		break;
	case SpecialType::WATER:
		setWaterRoute(_type);
		break;
	case SpecialType::TREE:
		setTreeRoute(_type);
		break;
	default:
		break;
	}

	circles.push_back(Circle(LEFT,RouteType(_type)));
	circles.push_back(Circle(RIGHT, RouteType(_type)));
}

void SpecialMinigame::updateTouch()
{
	auto touch = inputs.touch();
	auto handles = inputs.GetTouchHandleIDs();

	for (int i = 0;i < circles.size();i++) {
		circles[i].setIsSafe(false);//‚Ç‚Á‚¿‚àfalse‚É‚µ‚Æ‚­
	}
	for (auto id : handles) {
		if (inputs.isPressTouch(id, touch))
		{
			for (int i = 0;i < circles.size();i++) {
				if (circleCollision(touch.getPos(), circles[i].getPos(), circles[i].getSize()))
					circles[i].setIsSafe(true);//‚±‚±‚Å‚Ç‚Á‚¿‚àtrue‚É‚Å‚«‚½‚çtouch¬Œ÷
			}
		}
	}
	auto hand = inputzkoo.hand();
	for (auto& i : inputzkoo.GetHandleIDs())
	{
		if (inputzkoo.isRecognition(i, hand))
		{
			for (int i = 0;i < circles.size();i++) {
				if (circleCollision(hand.Position(), circles[i].getPos(), circles[i].getSize()))
					circles[i].setIsSafe(true);//‚±‚±‚Å‚Ç‚Á‚¿‚àtrue‚É‚Å‚«‚½‚çtouch¬Œ÷
			}
		}
	}
}
void SpecialMinigame::clearCircles()
{
	circles.clear();
}
void SpecialMinigame::startBackgroundEasing()
{
	backangle = 0.0f;
	if (start_t_x_ < 1.0f) {
		Easing::tCount(start_t_x_, 0.5f);
		backsize.x = app::getWindowWidth() / 20.f;
		backsize.y = EasingCubicIn(start_t_x_, 0, app::getWindowHeight() / 2);
	}
	if (start_t_x_ >= 1.0f) {
		Easing::tCount(start_angle_t_, 0.5f);
		backangle = EasingQuartIn(start_angle_t_, 0, 180);
		backsize.x = app::getWindowWidth() / 20.f;
		backsize.y = app::getWindowHeight() / 2;
	}
	if (start_angle_t_ >= 1.0f) {
		Easing::tCount(start_t_y_,0.5f);
		backsize.x = EasingQuadIn(start_t_y_, app::getWindowWidth() / 20.f, app::getWindowWidth() / 2);
		backsize.y = app::getWindowHeight() / 2;
	}
}
void SpecialMinigame::drawInfo()
{
	if (startend()) {
		gl::pushModelView();
		gl::translate(Vec2f(app::getWindowWidth() / 2, 250-150*info_t_));
		gl::color(ColorA(0, 0, 0, infoalfa_));
		cursor_->enableAndBind();
		gl::drawSolidRect(ci::Rectf(-infosize, infosize));
		cursor_->disable();
		gl::color(ColorA(1, 1, 1, 1));
		gl::popModelView();
	}
}
void SpecialMinigame::drawBackninja()
{
	if (!startend())return;
	Vec2f size = env.getWindowSize();
	gl::pushModelView();
	gl::translate(env.getWindowSize()/2);
	gl::color(ColorA(1, 1, 1, infoalfa_/1.2f));
	backninja_->enableAndBind();
	gl::drawSolidRect(ci::Rectf(-size/2,size/2));
	backninja_->disable();
	gl::color(ColorA(1, 1, 1, 1));
	gl::popModelView();
}
void SpecialMinigame::infoUpdate()
{
	if (startend()) {
		Vec2f startsize;
		startsize.x = env.getWindowWidth() / 2.0f;
		startsize.y = startsize.x*(float(cursor_->getHeight()) / float(cursor_->getWidth()));
		Vec2f endsize;
		endsize.x= env.getWindowWidth() / 3.0f;
		endsize.y = endsize.x*(float(cursor_->getHeight()) / float(cursor_->getWidth()));
		Easing::tCount(info_t_,1.0f);
		infosize.x= EasingBackIn(info_t_, startsize.x, endsize.x);
		infosize.y = EasingBackIn(info_t_, startsize.y, endsize.y);
		infoalfa_ = std::min(1.f, 2*info_t_);
	}
}
void SpecialMinigame::drawBackCircles()
{
	for (int i = 0;i < backcircles.size();i++) {
		gl::pushModelView();
		gl::translate(backcircles[i].pos);
		gl::scale(backcircles[i].size);
		gl::enableAlphaBlending();
		gl::color(ColorA(0, 0, 0, 0.2));
		gl::drawSolidCircle(Vec2f(0, 0), 1, 20);
		gl::color(ColorA(1, 1, 1, 1));
		gl::popModelView();
	}
}
void SpecialMinigame::setFireRoute(RouteType & routetype)
{
	switch (step_)
	{
	case 0:
		routetype = RouteType::ZEN;
		break;
	case 1:
		routetype = RouteType::ZAI;
		break;
	case 2:
		routetype = RouteType::RETU;
		break;
	default:
		break;
	}
}
void SpecialMinigame::setWaterRoute(RouteType & routetype)
{
	switch (step_)
	{
	case 0:
		routetype = RouteType::ZIN;
		break;
	case 1:
		routetype = RouteType::UI;
		break;
	case 2:
		routetype = RouteType::SHA;
		break;
	default:
		break;
	}
}
void SpecialMinigame::setTreeRoute(RouteType & routetype)
{
	switch (step_)
	{
	case 0:
		routetype = RouteType::TOU;
		break;
	case 1:
		routetype = RouteType::HYOU;
		break;
	case 2:
		routetype = RouteType::TEN;
		break;
	default:
		break;
	}
}

void SpecialMinigame::drawMyFillBox(Vec2f pos, Vec2f size)
{
	gl::pushModelView();
	gl::translate(pos);
	gl::translate(Vec2f(size.x/2,-size.y/2));
	gl::scale(size.x,-size.y);
	gl::drawSolidRect(Rectf(-Vec2f(0.5f,0.5f), Vec2f(0.5f, 0.5f)));
	gl::popModelView();
}

void SpecialMinigame::drawBackground()
{
	gl::pushModelView();
	gl::translate(app::getWindowWidth() / 2, app::getWindowHeight() / 2);
	gl::rotate(backangle);
	gl::color(ColorA(1,1,1,1));
	backimage_->enableAndBind();
	gl::drawSolidRect(ci::Rectf(-backsize, backsize));
	backimage_->disable();
	gl::popModelView();
}

int SpecialMinigame::getInObject()
{
	switch (type_)
	{
	case SpecialType::FIRE:
		if (step_ == 0) { return (int)RouteType::ZEN; }if (step_ == 1) { return(int)RouteType::ZAI; }if (step_ == 2) { return(int)RouteType::RETU; }
		break;
	case SpecialType::WATER:
		if (step_ == 0) { return(int)RouteType::ZIN; }if (step_ == 1) { return(int)RouteType::UI; }if (step_ == 2) { return(int)RouteType::SHA; }
		break;
	case SpecialType::TREE:
		if (step_ == 0) { return(int)RouteType::TOU; }if (step_ == 1) { return(int)RouteType::HYOU; }if (step_ == 2) { return(int)RouteType::TEN; }
		break;
	default:
		break;
	}
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