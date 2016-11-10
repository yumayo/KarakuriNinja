#pragma once
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
using namespace cinder;

class InObject {
public:
	InObject(){}
	InObject(int texnumber) {
		tex = loadImage(app::loadAsset("Textures/In/in"+std::to_string(texnumber)+".png"));
		pos_ = Vec2f(app::getWindowWidth() / 2, 300);
		flash_al_ = 0.0f;
		angle_ = 0.0f;
		angle_t_ = 0.0f;
		size_t_ = 0.0f;
		end_t_ = 0.0f;
		alfa_ = 0.0f;
		isdelete_ = false;
	}
	void draw() {
		gl::pushModelView();
		gl::enableAlphaBlending();
		gl::translate(Vec2f(app::getWindowWidth()/2,app::getWindowHeight()/2));
		gl::scale(Vec2f(app::getWindowWidth(), app::getWindowHeight()));
		gl::color(ColorA(1, 1, 1, flash_al_));
		gl::drawSolidRect(ci::Rectf(Vec2f(-0.5f,-0.5f), Vec2f(0.5f, 0.5f)));
		gl::popModelView();

		gl::pushModelView();
		gl::enableAlphaBlending();
		gl::translate(pos_);
		gl::rotate(angle_);
		gl::scale(size_);
		gl::color(ColorA(0, 0, 0, alfa_));
		tex.enableAndBind();
		gl::drawSolidRect(ci::Rectf(Vec2f(-0.5f, -0.5f), Vec2f(0.5f, 0.5f)));
		tex.disable();
		gl::popModelView();


	}
	void update() {
		if (angle_t_ < 1) {
			Easing::tCount(angle_t_,0.3f);
			size_.x = Easing::getEasing[Easing::EasingType::Linear](angle_t_, 200.f, 500.f);
			size_.y = Easing::getEasing[Easing::EasingType::Linear](angle_t_, 200.f, 500.f);
			angle_ = Easing::getEasing[Easing::EasingType::Linear](angle_t_,180,0.f);
			alfa_= Easing::getEasing[Easing::EasingType::Linear](angle_t_, 0, 1.f);
		}
		if (angle_t_ >=1.0f&&size_t_<1.0f)
		{
			Easing::tCount(size_t_, 0.5f);
			flash_al_ = Easing::getEasing[Easing::EasingType::Return](size_t_, 0, 1.f);
			size_.x= Easing::getEasing[Easing::EasingType::BackOut](size_t_, 500.f, 800.f);
			size_.y = Easing::getEasing[Easing::EasingType::BackOut](size_t_, 500.f, 800.f);
		}
		if(size_t_>=1.0f){
			end_t_ += 1.0f / (60.0f*0.5f);
			end_t_ = std::min(1.0f, end_t_);
			alfa_ = Easing::getEasing[Easing::EasingType::SineOut](end_t_, 1.0f,0.f);
		}
		if (end_t_ >= 1.0f) {
			isdelete_ = true;
		}
	}
	bool Mydelete() {
		return isdelete_;
	}
private:
	gl::Texture tex;
	Vec2f pos_;
	Vec2f size_;
	float angle_;
	float flash_al_;
	float angle_t_;
	float size_t_;
	float end_t_;
	float alfa_;
	bool isdelete_;
};