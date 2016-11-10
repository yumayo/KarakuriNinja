#include"Circle.h"
const int safeframe = 2;

void Circle::update()
{
	pushcount_++;
	if (pushcount_ % 3 == 0  && routes.size() < ROUTENUM) {
		routes.push_back(RouteCircle(routepos[routes.size()]));
	}
	for (auto&itr : routes) {
		itr.update();
	}
	if ((routes.size() == ROUTENUM)&&(movestart_== false)) {
		delay_++;
		if (delay_ > 1*80) {
			movestart_ = true;
		}
	}
	setRealSafe();
	move();
}

void Circle::draw()
{
	for (auto&itr : routes) {
		itr.draw();
	}
	if (isrealsafe_) {
		gl::color(ColorA(0, 1, 0,routes.size()/float(ROUTENUM)));
	}
	else {
		gl::color(ColorA(0, 1, 1, routes.size() / float(ROUTENUM)));
	}
	//if(isrealsafe_&&(!issafe_))
	//	gl::color(ColorA(0, 1, 0));

	gl::drawSolidCircle(pos_,size_);
	gl::color(ColorA(1, 1, 1, 1));

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

void Circle::setRoute()
{
	//ZEN, ZAI, RETU, ZIN, UI, SHA, TOU, HYOU, TEN
	switch (routetype_)
	{
	case RouteType::ZEN:
		setValue(Vec2f(200, 200), Vec2f(200, env.getWindowHeight()-300), Easing::EasingType::Linear, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth()-200, env.getWindowHeight( ) - 300 ), Vec2f(app::getWindowWidth() - 200, 200), Easing::EasingType::Linear, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::ZAI:
		setValue(Vec2f(200,550),Vec2f(500,200), Easing::EasingType::QuartIn, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 200, 200), Vec2f(app::getWindowWidth() - 500, 550), Easing::EasingType::QuartIn, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::RETU:
		setValue(Vec2f(500, 200), Vec2f(200, 550), Easing::EasingType::BounceOut, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 500, 550), Vec2f(app::getWindowWidth() - 200, 200), Easing::EasingType::BounceOut, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::ZIN:
		setValue(Vec2f(200, 200), Vec2f(200, env.getWindowHeight( ) - 300 ), Easing::EasingType::Linear, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 200, env.getWindowHeight( ) - 300 ), Vec2f(app::getWindowWidth() - 200, 200), Easing::EasingType::Linear, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::UI:
		setValue(Vec2f(200, 550), Vec2f(500, 200), Easing::EasingType::QuartIn, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 200, 200), Vec2f(app::getWindowWidth() - 500, 550), Easing::EasingType::QuartIn, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::SHA:
		setValue(Vec2f(500, 200), Vec2f(200, 550), Easing::EasingType::BounceOut, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 500, 550), Vec2f(app::getWindowWidth() - 200, 200), Easing::EasingType::BounceOut, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::TOU:
		setValue(Vec2f(200, 200), Vec2f(200, env.getWindowHeight( ) - 300 ), Easing::EasingType::Linear, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 200, env.getWindowHeight( ) - 300 ), Vec2f(app::getWindowWidth() - 200, 200), Easing::EasingType::Linear, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::HYOU:
		setValue(Vec2f(200, 550), Vec2f(500, 200), Easing::EasingType::QuartIn, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 200, 200), Vec2f(app::getWindowWidth() - 500, 550), Easing::EasingType::QuartIn, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	case RouteType::TEN:
		setValue(Vec2f(500, 200), Vec2f(200, 550), Easing::EasingType::BounceOut, Easing::EasingType::Linear,//LEFT�̏ꍇ
			Vec2f(app::getWindowWidth() - 500, 550), Vec2f(app::getWindowWidth() - 200, 200), Easing::EasingType::BounceOut, Easing::EasingType::Linear);//RIGHT�̏ꍇ
		break;
	default:
		break;
	}
	float t = 0.0f;
	for (int i = 0;i < ROUTENUM;i++) {
		routepos[i].x = Easing::getEasing[easingtype_x_](t, startpos_.x,endpos_.x);
		routepos[i].y = Easing::getEasing[easingtype_y_](t, startpos_.y, endpos_.y);
		t += 1.0f / (float(ROUTENUM-1));
	}
}



void Circle::setValue(Vec2f leftstartpos, Vec2f leftendpos, Easing::EasingType left_type_x, Easing::EasingType left_type_y, Vec2f rightstartpos, Vec2f rightendpos, Easing::EasingType right_type_x, Easing::EasingType right_type_y)
{
	if (type_ == LEFT) {
		startpos_ = leftstartpos;
		endpos_ = leftendpos;
		easingtype_x_ = left_type_x;
		easingtype_y_ = left_type_y;
	}
	else {
		startpos_ = rightstartpos;
		endpos_ = rightendpos;
		easingtype_x_ = right_type_x;
		easingtype_y_ = right_type_y;
	}
}

void Circle::move()
{
	if (movestart_) {
		Easing::tCount(move_t_,3.0f);
	}
	pos_.x = Easing::getEasing[easingtype_x_](move_t_, startpos_.x, endpos_.x);
	pos_.y = EasingLinear(move_t_, startpos_.y, endpos_.y);
}
