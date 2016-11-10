#pragma once
#pragma once
#include"../Special/SpecialSceneBase.h"



class NotSpecial : public SpecialSceneBase {
public:
	NotSpecial() {

	}
	void draw() override {
		//何もしません
	}
	void update() override {
		//何もしません
	}
	bool shift() override {
		return false;
	}
	bool isFullScreen() {
		return false;
	}
private:

};