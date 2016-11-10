#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"cinder/Rand.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"
#include"../Special/Circle.h"
#include"../Special/InObject.h"
#include"../Special/Gage.h"
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
		start_t_x_ = 0.f;
		start_t_y_ = 0.f;
		info_t_ = 0.0f;
		backimage_ = loadImage(app::loadAsset("minihaikei.png"));
		cursor_= loadImage(app::loadAsset("awasero.png"));
		backninja_ = loadImage(app::loadAsset("backninja.png"));
		start_angle_t_ = 0.0f;
		ispush_back_circles_ = false;
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
		return startend();
	}
private:
	struct BackCircle {
		Vec2f pos;
		Vec2f size;
	};
	std::vector<Circle> circles;
	std::vector<BackCircle> backcircles;
	std::vector<InObject> inobject;
	Gage gage_;
	SpecialType type_;
	int step_;
	int success_count_;
	bool go_next_;
	gl::Texture backimage_;
	gl::Texture cursor_;
	gl::Texture backninja_;
	Vec2f backsize;
	float backangle;
	Vec2f infosize;
	float infoalfa_;
	float start_t_x_;
	float start_t_y_;
	float start_angle_t_;
	float info_t_;
	bool ispush_back_circles_;

	void create2Circles();
	bool circleCollision(const Vec2f pos1, const Vec2f pos2, const float r_size, float level = 1.f);
	void updateTouch();
	bool isSuccessTouchDoubleCircle() { return (circles[0].getIsRealSafe() && circles[1].getIsRealSafe()); }
	void clearCircles();
	void startBackgroundEasing();
	void drawInfo();
	void drawBackninja();
	void infoUpdate();
	void drawBackCircles();
	void setFireRoute(RouteType& routetype);
	void setWaterRoute(RouteType& routetype);
	void setTreeRoute(RouteType& routetype);
	void drawMyFillBox(Vec2f pos, Vec2f size);
	void drawBackground();
	int getInObject();
	bool startend() {
		return start_t_y_ >= 1.0f;
	}
};