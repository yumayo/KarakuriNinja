#include"Special.h"
void Special::update(bool canspecial,bool isstop) {
	/////////////////////
	//if (isspesial == false&&canspecial) {
	//	auto touch = inputs.touch();
	//	auto ids = inputs.GetTouchHandleIDs();

	//	for (auto id : ids)
	//	{

	//		//////とりあえずこれでプレイヤーアイコンの近くをプッシュするとスペシャルモードに入れるようにしておきました
	//		if (inputs.isPushTouch(id, touch))
	//		{
	//			Vec2f iconpos = Vec2f(0, env.getWindowHeight())+ Vec2f(10 + 128, -10 - 128);
	//			float iconsize = 128.f;
	//			float kyori = (iconpos.x - touch.getPos().x)*(iconpos.x - touch.getPos().x) + (iconpos.y - touch.getPos().y)*(iconpos.y - touch.getPos().y);
	//			if (kyori < (iconsize*iconsize)) {
	//					goSpecialMode();
	//			}
	//		}
	//		//////
	//	}

	//	auto hand = inputzkoo.hand();
	//	for (auto& i : inputzkoo.GetHandleIDs())
	//	{
	//		if (inputzkoo.isPush(i, hand))
	//		{
	//			Vec2f iconpos = Vec2f(0, env.getWindowHeight()) + Vec2f(10 + 128, -10 - 128);
	//			float iconsize = 128.f;
	//			float kyori = (iconpos.x - hand.Position().x)*(iconpos.x - hand.Position().x) + (iconpos.y - hand.Position().y)*(iconpos.y - hand.Position().y);
	//			if (kyori < (iconsize*iconsize)) {
	//				goSpecialMode();
	//			}
	//		}
	//	}

	//}

	/////////////
	special_->update(isstop);
	effectend_ = false;
	if (special_->shift())
		shift();
}
void Special::draw() {
	special_->draw();
}

void Special::shift()
{
	switch (mode)
	{
	case Special::NOTSPECIAL:
        //何もしません
		break;
	case Special::SELECT:
		special_ = std::make_shared<SpecialMinigame>(special_->getSpecialType());
		mode = MODE::MINIGAME;
		break;
	case Special::MINIGAME:
		specialpower_ = special_->getSpecialPower();
		special_ = std::make_shared<SpecialEffect>(special_->getSpecialType());
		mode = MODE::EFFECT;
		break;
	case Special::EFFECT:
		special_type_ = special_->getSpecialType();
		special_ = std::make_shared<SubTime>(special_type_);
		mode = SUBTIME;
		isspesial = false;
		effectend_ = true;
		break;
	case Special::SUBTIME:
		special_type_ = special_->getSpecialType();
		special_ = std::make_shared<NotSpecial>();
		mode = NOTSPECIAL;
		break;
	default:
		break;
	}
}

