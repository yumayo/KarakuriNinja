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

class Water {
public:
	Water() {}
	Water(gl::Texture tex, gl::Texture tex2, User::Audio* se ) :se_( se ) {
		end = false;
		count = 0;
		tex_ = tex;
		texsize = Vec2f(1200, 700);
        se_->Play( );
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
	bool end;
	void drawBigWater();
    User::Audio* se_;
};