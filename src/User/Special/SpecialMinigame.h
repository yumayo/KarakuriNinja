#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"
#include"../Special/Circle.h"
using namespace cinder;
class SpecialMinigame : public SpecialSceneBase {
public:
	SpecialMinigame(){
	}
	SpecialMinigame(SpecialType type) {
		success_count_ = 0;
		step_ = 0;
		go_next_ = false;
		type_ = type;
		create2Circles();
	}
	void draw() override;
	void update() override;
	bool shift() override {
		return go_next_;
	}
	SpecialType getSpecialType() override {
		return type_;
	}
	bool isFullScreen() {
		return false;
	}
private:
	std::vector<Circle> circles;
	void create2Circles();
	int step_;
	int success_count_;
	bool go_next_;
	bool circleCollision(const Vec2f pos1, const Vec2f pos2, const float r_size, float level = 1.f);
	void updateTouch();
	bool isSuccessTouchDoubleCircle() { return (circles[0].getIsRealSafe() && circles[1].getIsRealSafe()); }
	void clearCircles();
	SpecialType type_;
};