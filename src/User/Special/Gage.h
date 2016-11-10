#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Utilitys/Nomoto.h"
#include"../Utilitys/Audio.h"
#include"GlobalData.hpp"
using namespace cinder;
class Gage {
public:
	Gage(){
		issuccces_[0] = false;
		issuccces_[1] = false;
		count_ = 0.0f;
		texsname[WAKU] = "spwaku.png";
		texsname[GAGE] = "spgage.png";
		for (int i = 0;i < texs::MAX;i++) {

			texs[i] = &GData::FindTexture("Textures/"+texsname[i]);
		}
		maxflag = false;
		max_t = 0.0f;
		se = &GData::FindAudio("SE/chargemax.wav");
	}
	void setisCount(bool is1,bool is2) {
	      issuccces_[0] = is1;
		  issuccces_[1] = is2;
	}
	void draw();
	void update();
	float getCount() {
		return std::max(0.1f,count_);
	}
private:
	enum texs {
		WAKU,GAGE,MAX
	};
	std::string texsname[2];
	Vec2f pos_;
	Vec2f maxsize_;
	Vec2f size_;
	User::Audio* se;
	float count_;
	bool maxflag;
	float max_t;
	bool issuccces_[2];
	gl::Texture* texs[2];
	void gagedraw(cinder::gl::Texture const & texture,cinder::Vec2f maxsize,cinder::Vec2f position, cinder::Area area, float size);
};