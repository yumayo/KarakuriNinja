#pragma once 
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
//! @file SlashEffect 
//! @brief ����Ő؂铮���������Ƃ��� 
//!�@�@�@�@ ����ɍ��킹��Effect�̕`������܂� 
//! @note 
//!       �傿��񂪂�����Ƃ���ł� 
//! @date 2016-07-12 
//! @author Taka Nomoto 


class Slash
{
public:
	Slash() : isActive(false), maxactiveTime(30),
		activeTime(0)
	{
		using namespace cinder;
		tex = loadImage(app::loadAsset("slash.png"));
		delete_t = 0.0f;
	}
	//Effect�̕`���ݒ肵�܂� 
	void Set(ci::Vec2f pos_, ci::Vec2f size_, float angle_)
	{
		pos = pos_;
		size = size_;
		angle = angle_;
		activeTime = 0;
		isActive = true;
	}

	void Update();
	void Draw();
	bool Active() { return isActive; }
private:
	ci::Vec2f pos;
	ci::Vec2f size;
	ci::Vec2f realsize;
	ci::Vec2f realpos;
	float angle;
	float delete_t;
	int activeTime;
	int maxactiveTime;
	cinder::gl::Texture tex;
	bool isActive;
	void textureDraw(cinder::gl::Texture const& texture, cinder::Vec2f position = cinder::Vec2f::zero());
	void textureDraw(cinder::gl::Texture const& texture, cinder::Vec2f position, cinder::Area area, float value);
	float EasingLinear(float t, float b, float e) {
		return (e - b) * t + b;

	}

};
