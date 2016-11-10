#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/RouteCircle.h"
#include"GlobalData.hpp"
using namespace cinder;
enum CircleType {
	LEFT,RIGHT
};
enum RouteType {
	ZEN, ZAI, RETU, ZIN, UI, SHA, TOU,HYOU,TEN
};
const int ROUTENUM = 15;
class Circle {
public:
	Circle() {}
	Circle(const CircleType type,RouteType routetype) {
		size_ = 160.f;
		type_ = type;
		routetype_ = routetype;
		issafe_ = false;
		isrealsafe_ = false;
		safecount_ = 0;
		pushcount_ = 0;
		move_t_ = 0.0f;
		delay_ = 0;
		movestart_ = false;
		setRoute();
		tex = &GData::FindTexture("ac-sp.png");
	}
	void update();
	void draw();
	std::vector<RouteCircle> routes;
	Vec2f getPos() { return pos_; }
	float getSize() { return size_;}
	CircleType getType() { return type_; }
	bool getIsSafe() { return issafe_; }//仮のセーフこれが数フレームfalseだと
	bool getIsRealSafe() { return isrealsafe_; }//これがfalseになります
	void setIsSafe(bool is) { issafe_ = is; }
	bool getClear() {
		return move_t_ >= 1.0f;
	}
	bool getMoveStart() {
		return movestart_;
	}

private:
	Vec2f pos_;
    float size_;
	CircleType type_;
	cinder::gl::Texture* tex;
	Easing::EasingType easingtype_x_;
	Easing::EasingType easingtype_y_;
	bool issafe_;
	bool isrealsafe_;
	int safecount_;
	RouteType routetype_;
	void setRealSafe();
	void setRoute();
	Vec2f routepos[ROUTENUM];
	Vec2f startpos_;
	Vec2f endpos_;
	int delay_;
	bool movestart_;
	void setValue(Vec2f leftstartpos, Vec2f leftendpos, Easing::EasingType left_type_x, Easing::EasingType left_type_y,
		Vec2f rightstartpos, Vec2f rightendpos, Easing::EasingType right_type_x, Easing::EasingType right_type_y);
	int pushcount_;
	void move();
	float move_t_;

};