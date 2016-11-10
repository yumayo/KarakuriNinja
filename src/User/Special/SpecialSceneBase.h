#pragma once
enum SpecialType {
	FIRE, WATER, TREE,NOTSELECTED
};
class SpecialSceneBase {
 public:
	 void virtual update(bool isstop) {
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
	bool virtual Effecting() {
		return false;
	}
	float virtual getSpecialPower() {
		return 0;
	}
};