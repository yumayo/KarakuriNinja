#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"
#include"../Utilitys/Yumayo.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"SpecialName.h"
#include"Fire.h"
#include"Water.h"
#include"Tree.h"
using namespace cinder;

class SpecialEffect : public SpecialSceneBase {
public:
	SpecialEffect(){}
	SpecialEffect(SpecialType type){
		specialtype_ = type;
		camera_move_ = false;
		go_next_ = false;
		count_ = 0;
		angle_ = 0.f;
		roadTex();
		namesize = Vec2f(100,100);
		effect[0]= loadImage(app::loadAsset("Textures/Effect/effect"+std::to_string(2*int(type))+".png"));
		effect[1] = loadImage(app::loadAsset("Textures/Effect/effect" + std::to_string(2*int(type)+1) + ".png"));
		makimono= loadImage(app::loadAsset("Textures/makimono.png"));
	}
	void draw() override;
	void update() override;
	bool shift() override {
		return go_next_;
	}
	SpecialType getSpecialType() override {
		return specialtype_;
	}
	bool isFullScreen() override {
		return false;
	}
	bool Effecting() override {
		return camera_move_;
	}
private:
	SpecialType specialtype_;
	bool go_next_;
	int count_;
	float angle_;
	bool camera_move_;
	void pushNames();
	gl::Texture texs[7];
	gl::Texture makimono;
	std::vector<SpecialName> names;
	gl::Texture effect[2];
	std::vector<Fire> fire;
	std::vector<Water> water;
	std::vector<Tree> tree;
	void pushEffect();
	void goShift();
	void drawMakimono();
	ColorA namecolor_;
	Vec2f namepos[7];
	Vec2f namesize;
	void roadTex();
};
