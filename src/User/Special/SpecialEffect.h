#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"
using namespace cinder;

class SpecialEffect : public SpecialSceneBase {
public:
	SpecialEffect(){}
	SpecialEffect(SpecialType type) {
		specialtype_ = type;
		go_next_ = false;
		count_ = 0;
		angle_ = 0.f;
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
private:
	SpecialType specialtype_;
	bool go_next_;
	int count_;
	float angle_;
};
