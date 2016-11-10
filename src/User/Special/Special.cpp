#include"Special.h"
void Special::update(bool canspecial) {
	/////////////////////
	if (isspesial == false&&canspecial) {
		auto touch = inputs.touch();
		auto ids = inputs.GetTouchHandleIDs();

		for (auto id : ids)
		{

			//////�Ƃ肠��������Ńv���C���[�A�C�R���̋߂����v�b�V������ƃX�y�V�������[�h�ɓ����悤�ɂ��Ă����܂���
			if (inputs.isPushTouch(id, touch))
			{
				Vec2f iconpos = Vec2f(0, env.getWindowHeight())+ Vec2f(10 + 128, -10 - 128);
				float iconsize = 128.f;
				float kyori = (iconpos.x - touch.getPos().x)*(iconpos.x - touch.getPos().x) + (iconpos.y - touch.getPos().y)*(iconpos.y - touch.getPos().y);
				if (kyori < (iconsize*iconsize)) {
						goSpecialMode();
				}
			}
			//////
		}
	}

	/////////////
	special_->update();
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
        //�������܂���
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

