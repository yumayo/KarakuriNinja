#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
class SpecialShuriken
{
public:
	SpecialShuriken();
	SpecialShuriken(cinder::Vec2f pos, cinder::Vec2f size, cinder::gl::Texture* tex);
	void draw(cinder::Vec2f trancepos);
	void update(cinder::Vec2f speed);
	bool end() {
		return pos_.x > env.getWindowWidth()*1.2;
	};
private:
	cinder::Vec2f pos_;
	cinder::Vec2f size_;
	cinder::gl::Texture* tex_;
	float rotate;
};