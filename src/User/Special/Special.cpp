#include"Special.h"
void Special::update() {
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
        //‰½‚à‚µ‚Ü‚¹‚ñ
		break;
	case Special::SELECT:
		special_ = std::make_shared<SpecialMinigame>(special_->getSpecialType());
		mode = MODE::MINIGAME;
		break;
	case Special::MINIGAME:
		special_ = std::make_shared<SpecialEffect>(special_->getSpecialType());
		mode = MODE::EFFECT;
		break;
	case Special::EFFECT:
		special_type_ = special_->getSpecialType();
		special_ = std::make_shared<NotSpecial>();
		mode = NOTSPECIAL;
		isspesial = false;
		effectend_ = true;
		break;
	default:
		break;
	}
}

