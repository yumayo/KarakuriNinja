#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
using namespace cinder;

class SmallFire {
public:
	SmallFire() {}
	SmallFire(gl::Texture tex) {
		image = tex;
		count = 0;
	}
	void update();
	void draw();
private:
	gl::Texture image;
	int count;
};