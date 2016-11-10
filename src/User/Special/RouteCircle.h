#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
using namespace cinder;
class RouteCircle {
public:
	RouteCircle(){}
	RouteCircle(Vec2f pos) {
		pos_ = pos;
		size_ = 10.f;
		color_ = ColorA(1, 1,0, 0);
		alfa_t_ = 0.0f;
		count_ = 0;
		isdelete = false;
	}
	void draw() {
		gl::pushModelView();
		gl::enableAlphaBlending();
		gl::translate(pos_);
		gl::color(color_);
		gl::drawSolidCircle(Vec2f(0,0),size_);
		gl::popModelView();
		gl::color(ColorA(1,1,1,1));
	}
	void update() {
		if (alfa_t_ < 0.5f&&(!isdelete)) {
			alfa_t_ += 0.5f / (60.f*0.3f);
			alfa_t_ = std::min(0.5f, alfa_t_);
			color_ = ColorA(1, 1, 0, alfa_t_);
		}
		else {
			count_++;
			if (count_ > 150) {
				isdelete = true;
			}
		}
		if (isdelete) {
			alfa_t_ -= 0.5f / (60.f*1.25f);
			alfa_t_ = std::max(0.f, alfa_t_);
			color_ = ColorA(1, 1, 0, alfa_t_);
		}
	}
private:
	Vec2f pos_;
	float size_;
	ColorA color_;
	float alfa_t_;
	int count_;
	bool isdelete;
};