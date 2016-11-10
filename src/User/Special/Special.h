
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include"SpecialSceneBase.h"
#include"SpecialSelect.h"
#include"SpecialMinigame.h"
#include"SpecialEffect.h"
#include"NotSpecial.h"
#include"SubTime.h"
#include<memory>

class Special {
public:
	Special() {
		isspesial = false;
		mode = NOTSPECIAL;
		special_type_ = NOTSELECTED;
		special_ = std::make_shared<NotSpecial>();
		effectend_ = false;
	}
	void update();
	void draw();
	
	//スペシャルを選択するモードに入る時に使います、一応サブ効果中は入れないようにしておきました
	void goSpecialMode() {
		if (special_type_ == NOTSELECTED) {
			isspesial = true;
			special_ = std::make_shared<SpecialSelect>();
			mode = SELECT;
		}
	}

	//スペシャルセレクトに入ってからエフェクトが終わるまで間TRUEが帰ります。
	//プレイヤーの攻撃やガードのupdate、敵のupdateを止めるなどに使ってください
	bool getIsSpecial() {
		return isspesial;
	}

	//ミニゲーム中は画面全体を使うためマップや敵が見えない予定なので、
	//もし重くなったら軽減処理のためこのフラグで描画をとめておいてください。通常時：false,セレクト：false,ミニゲーム:true,エフェクト:false
	bool isMinigame() {
		return special_->isFullScreen();
	}

	//選んだスペシャルを取得します。エフェクトが終わって初めてFIREなどが入ります
	SpecialType getSpecialType() {
		return special_type_;
	}
	//サブ効果の時間が終わったらこれを呼んでください
	void resetSpecial() {
		special_type_ = NOTSELECTED;
	}
	//現在プレイヤークラスでこれを呼んでいないため、サブ効果の時間が無限みたいな状況なので
	//一度スペシャルを使うと２度とスペシャルを使えない感じになっています


	//エフェクト中に敵がダメージをくらっている感じを出すときなどに使ってください
	bool isEffecting() {
		return mode == MODE::EFFECT;
	}

	//エフェクトが終わった瞬間の１フレームだけTRUEが帰ります
	//敵全体にダメージを与える処理などに使ってください
	bool getEffectEnd() {
		return effectend_;
	}
private:
	std::shared_ptr<SpecialSceneBase>special_;
	void shift();
	enum MODE {
		NOTSPECIAL, SELECT, MINIGAME, EFFECT,SUBTIME
	};
	MODE mode;
	bool isspesial;
	bool effectend_;
	SpecialType special_type_;
};