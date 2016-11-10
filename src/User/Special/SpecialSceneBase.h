#pragma once
enum SpecialType {
	FIRE, WATER, TREE,NOTSELECTED
};
class SpecialSceneBase {
 public:
	 void virtual update() {
	 };
	 void virtual draw() {};
	bool virtual shift() {
		return false;
	};
	void virtual setSpecial(bool is) {

	};
	SpecialType virtual getSpecialType() {
		return NOTSELECTED;
	}
	bool virtual isFullScreen() {
		return false;
	}
};