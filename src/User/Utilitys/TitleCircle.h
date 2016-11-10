#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"../Utilitys/Audio.h"

class TitleCircle {
public:
	TitleCircle();
	TitleCircle(cinder::Vec2f pos,cinder::Vec2f size,cinder::gl::Texture* tex);
	void draw(float posx);
	void update(float y_speed);
private:
	cinder::Vec2f pos_;
	cinder::Vec2f size_;
	cinder::gl::Texture* tex_;
	float rotate;
};