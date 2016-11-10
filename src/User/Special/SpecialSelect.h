
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"
#include"ZKOO.hpp"
#include"../Utilitys/Audio.h"
#include"GlobalData.hpp"
using namespace cinder;
const int ICONNUM = 3;
class SpecialSelect : public SpecialSceneBase{
public:
	SpecialSelect::SpecialSelect(){
		is_special = false;
		start_t = 0.0f;
		go_next_ = false;
		ischoosed_ = false;
		end_t_ = 0.0f;
		last_t_ = 0.0f;
		isshifteasing_ = false;
		for (int i = 0;i < ICONNUM;i++) {
			Icon icon;
			icon.startpos = Vec2f(app::getWindowWidth() / 2 + float((i - 1)*450.f), app::getWindowHeight() / 2 + 700);
			icon.pos = icon.startpos;
			icon.endpos = icon.startpos + Vec2f(0, -800);
			icon.shiftpos = icon.startpos;
			icon.size = Vec2f(250.f, 250.f);
			icon.angle_t = 0.0f;
			icon.startangle = 5.f;
			icon.opened = false;
			icons.push_back(icon);
		}
		for (int i = 0;i < 3;i++) {
			icontex[i] = &GData::FindTexture("Textures/select"+std::to_string(i)+".png");
		}
		open_se.push_back(&GData::FindAudio("SE/hiraku.wav"));
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
	struct Icon {
		Vec2f pos;//実際のposです
		Vec2f startpos;
		Vec2f endpos;
		float startangle;
		float angle_t;
		float angle;
		Vec2f size;//四角形としてのサイズです。円なら半分にしてください
		Vec2f shiftpos;
		bool opened;
	};
	std::vector<User::Audio*> open_se;
	std::vector<Icon>icons;
	void drawIcon();
	Vec2f iconsize;
	bool is_special;
	bool isshifteasing_;
	bool go_next_;
	bool ischoosed_;
	SpecialType specialtype_;
	void updateIcon();
	float start_t;
	float end_t_;
	float last_t_;
	gl::Texture* icontex[3];
	bool isCanChoose() {
		return (icons[icons.size() - 1].angle_t >= 1.0f)&&(ischoosed_==false);
	}
	bool circleCollision(const Vec2f pos1, const Vec2f pos2, const float r_size, float level = 1.f);
	void chooseSpecial();//とりあえずタッチのプッシュでやっておきました
};