#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
using namespace cinder;

class Fire {
public:
	Fire(){}
	Fire(gl::Texture tex, gl::Texture tex2){
		end = false;
		count = 0;
		tex_ = tex;
		texsize = Vec2f(1200, 700);
	}
	void update();
	void draw();
	bool getend() {
		return end;
	}
private:
	gl::Texture tex_;
	int count;
	Vec2f texsize;
	void pushFire();
	bool end;
	void drawBigFire();
};