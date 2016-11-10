#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"

class SubTime : public SpecialSceneBase {
public:
	SubTime() {

	}
	SubTime(SpecialType _specialtype) {
		specialtype = _specialtype;
		setSpecialTime(specialtype);
		count_ = 0;
		subend_ = false;
	}
	void draw() override {
		//‰½‚à‚µ‚Ü‚¹‚ñ
	}
	void update(bool isstop) override {
		if (specialtype != SpecialType::NOTSELECTED) {
			if(!isstop)
			count_++;
			if (count_ > specialtime_) {
				specialtype = SpecialType::NOTSELECTED;
				subend_ = true;
			}
		}
	}
	SpecialType getSpecialType() override {
		return specialtype;
	}
	bool shift() override {
		return subend_;
	}
	bool isFullScreen() {
		return false;
	}
private:
	SpecialType specialtype;
	int specialtime_;
	int count_;
	bool subend_;
	void setSpecialTime(SpecialType type) {
		switch (type)
		{
		case NOTSELECTED:
			specialtime_ = 60 * 10000;//–¢’è‹`–hŽ~
			break;
		case FIRE:
			specialtime_ = 60 * 15;
			break;
		case WATER:
			specialtime_ = 60 * 15;
			break;
		case TREE:
			specialtime_ = 60 * 15;
			break;
		default:
			break;
		}
	}
};