#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Utilitys/Nomoto.h"
#include"GlobalData.hpp"
using namespace cinder;
class Gage {
public:
	Gage(){
		issuccces_ = false;
		count_ = 0.0f;
		texsname[WAKU] = "spwaku.png";
		texsname[GAGE] = "spgage.png";
		for (int i = 0;i < texs::MAX;i++) {

			texs[i] = &GData::FindTexture("Textures/"+texsname[i]);
		}
	}
	void setisCount(bool is) {
	      issuccces_ = is;
	}
	void draw();
	void update();
	float getCount() {
		return count_;
	}
private:
	enum texs {
		WAKU,GAGE,MAX
	};
	std::string texsname[2];
	Vec2f pos_;
	Vec2f maxsize_;
	Vec2f size_;
	float count_;
	bool issuccces_;
	gl::Texture* texs[2];
	void gagedraw(cinder::gl::Texture const & texture,cinder::Vec2f maxsize,cinder::Vec2f position, cinder::Area area, float size);
};