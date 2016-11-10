#include"../Special/SpecialEffect.h"

const float end_y_pos = 100.f;
void SpecialEffect::draw()
{
	gl::pushModelView();
	for (auto &itr : fire) {
		itr.draw();
	}
	for (auto &itr : water) {
		itr.draw();
	}
	for (auto &itr : tree) {
		itr.draw();
	}
	drawMakimono();
	for (int i = 0;i < names.size();i++) {
		names[i].draw();
	}

	gl::popModelView();
}

void SpecialEffect::update()
{
	if (names.size() == 7) {
		if (names[names.size()-1].getT() >= 1.0f) {
			count_++;
			camera_move_ = true;
		}
	}
	
	pushNames();
	for (int i = 0;i < names.size();i++) {
		names[i].update();
	}
	pushEffect();

	for (auto &itr : fire) {
		itr.update();
	}
	for (auto &itr : water) {
		itr.update();
	}
	for (auto &itr : tree) {
		itr.update();
	}
	goShift();
}

void SpecialEffect::pushNames()
{
	if (names.size() == 7) {
		return;
	}
	if (names.size() == 0) {
			names.push_back(SpecialName(texs[names.size()], Vec2f((names.size())*namesize.x+ app::getWindowWidth()/2-3*namesize.x, end_y_pos), namesize,namecolor_));
	}else {
		if (names[names.size() - 1].getT() > 0.6f) {
				names.push_back(SpecialName(texs[names.size()], Vec2f((names.size())*namesize.x + app::getWindowWidth() / 2 - 3 * namesize.x, end_y_pos), namesize, namecolor_));
		}
	}
}

void SpecialEffect::pushEffect()
{
	switch (specialtype_)
	{
	case SpecialType::FIRE:
		if (names.size() == 7 && fire.size() == 0) {
			if (names[names.size() - 1].getT() >= 1.0f)
			fire.push_back(Fire(effect[0], effect[1]));
		}
		break;
	case SpecialType::WATER:
		if (names.size() == 7 && water.size() == 0) {
			if (names[names.size() - 1].getT() >= 1.0f)
			water.push_back(Water(effect[0], effect[1]));
		}
		break;
	case SpecialType::TREE:
		if (names.size() == 7 && tree.size() == 0) {
			if (names[names.size() - 1].getT() >= 1.0f)
			tree.push_back(Tree(effect[0], effect[1]));
		}
		break;
	default:
		break;
	}

}

void SpecialEffect::goShift()
{
	if (fire.size() == 1) {
		if (fire[0].getend())
			go_next_ = true;
	}
	if (water.size() == 1) {
		if (water[0].getend())
			go_next_ = true;
	}
	if (tree.size() == 1) {
		if (tree[0].getend())
			go_next_ = true;
	}
}

void SpecialEffect::drawMakimono()
{
	gl::pushModelView();
	gl::translate(Vec2f(app::getWindowWidth() / 2, end_y_pos));
	gl::scale(Vec2f(1100,300));
	makimono.enableAndBind();
	gl::drawSolidRect(Rectf(Vec2f(-0.5,-0.5),Vec2f(0.5,0.5)));
	makimono.disable();
	gl::popModelView();
}

void SpecialEffect::roadTex()
{
	std::string sp_type;
	switch (specialtype_)
	{
	case SpecialType::FIRE:
		sp_type = "hi";
		namecolor_ = ColorA(0.7,0,0,1);
		break;
	case SpecialType::WATER:
		sp_type = "sui";
		namecolor_ = ColorA(0, 0, 0.7, 1);
		break;
	case SpecialType::TREE:
		sp_type = "mo";
		namecolor_ = ColorA(0, 0.3, 0, 1);
		break;
	default:
		break;
	}
	for (int i = 0;i < 7;i++) {
		texs[i] = loadImage(app::loadAsset("Textures/spname/"+sp_type+std::to_string(i)+".png"));
	}
}
