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

class Fire {
public:
	Fire(){}
	Fire(gl::Texture tex, gl::Texture tex2,User::Audio* se):se_(se){
		end = false;
		count = 0;
		tex_ = tex;
		texsize = Vec2f(1200, 700);
		se->Play();
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
	User::Audio* se_;
};