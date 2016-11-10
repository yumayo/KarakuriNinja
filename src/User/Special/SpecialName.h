#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"../Utilitys/Audio.h"
using namespace cinder;

class SpecialName {
public:
	SpecialName() {}
	SpecialName(gl::Texture tex,Vec2f endpos,Vec2f endsize, ColorA color,User::Audio* _se) :se(_se){
		tex_ = tex;
		color_ = color;
		endsize_ = endsize;
		endpos_ = endpos;
		t_ = 0.f;
		seend_ = false;
	}
	void update();
	void draw();
	float getT() {
		return t_;
	}
private:
	Vec2f pos_;
	Vec2f size_;
	Vec2f endpos_;
	Vec2f endsize_;
	gl::Texture tex_;
	ColorA color_;
	float t_;
	bool seend_;
	User::Audio* se;
};