
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"GlobalData.hpp"

class HundAnimation{
public:
	HundAnimation();
	void updateAttack(cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size);
	void updateGuard(cinder::Vec2f leftpos, cinder::Vec2f rightpos, cinder::Vec2f size);
	void drawAttack(cinder::Vec2f linesize);
	void drawGuard(cinder::Vec2f linesize);
	///ƒJƒEƒ“ƒgŒn‚ğ‚·‚×‚Ä‰Šú‰»
	void Reset();
private:
	cinder::Vec2f attack_hund_pos;
	cinder::Vec2f guard_right_pos;
	cinder::Vec2f guard_left_pos;
	cinder::Vec2f handsize;
	cinder::gl::Texture* righthandopen;
	cinder::gl::Texture* righthandclose;
	cinder::gl::Texture* lefthandopen;
	cinder::gl::Texture* lefthandclose;
	cinder::gl::Texture* attackhand;
	cinder::gl::Texture* rightguardhand;
	cinder::gl::Texture* leftguardhand;
	cinder::gl::Texture* yazirushi;
	cinder::gl::Texture* guardyazirushi;
	cinder::Vec2f attackcenterpos;
	cinder::Vec2f guardcenterpos;
	int attackframe;
	int guardframe;
	float movet_;
	float attackdelay_;
	float attackangle;
	float guardangle;
	bool isattackanimation;
	void selectGuardTexture();
	void selectAttackTexture();
	void moveAttackPos(cinder::Vec2f startpos,cinder::Vec2f endpos,cinder::Vec2f size);
	void attackReset();
	void setcenterpos(cinder::Vec2f centerpos, cinder::Vec2f& pos);
};