#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
using namespace cinder;
enum CircleType {
	LEFT,RIGHT
};
enum RouteType {
	ZAI, RETU, KAI, SHA, TOU, HYOU, RIN,
};
class Circle {
public:
	Circle() {}
	Circle(const Vec2f pos,const CircleType type) {
		pos_ = pos;
		size_ = 100.f;
		type_ = type;
		issafe_ = false;
		isrealsafe_ = false;
		safecount_ = 0;
	}
	void update();
	void draw();
	Vec2f getPos() { return pos_; }
	float getSize() { return size_;}
	CircleType getType() { return type_; }
	bool getIsSafe() { return issafe_; }//仮のセーフこれが数フレームfalseだと
	bool getIsRealSafe() { return isrealsafe_; }//これがfalseになります
	void setIsSafe(bool is) { issafe_ = is; }
private:
	Vec2f pos_;
    float size_;
	CircleType type_;
	bool issafe_;
	bool isrealsafe_;
	int safecount_;
	void setRealSafe();
};