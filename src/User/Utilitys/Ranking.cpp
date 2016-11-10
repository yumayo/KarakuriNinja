#include"Ranking.h"

using namespace ci;

namespace User
{
	Ranking::Ranking():font(u8"HGs‘‘Ì", 85)
	{
		roadtext();
		ranking_ = player_scores.size() + 1;
		for (int i = 0;i < 10;i++) {
			num1[i] = &GData::FindTexture("Textures/renzan/combo-1-" + std::to_string(i) + ".png");
		}
		for (int i = 1;i < 10;i++) {
			num10[i - 1] = &GData::FindTexture("Textures/renzan/combo-10-" + std::to_string(i) + ".png");
		}
		rank = &GData::FindTexture("Textures/renzan/rank.png");
		frame = 0;
		rankbuf = ranking_;
		saverank = ranking_;
		t = 1.0f;
		endt_ = 1.0f;
	}

	void Ranking::update(int score, int maxcombo, int nowhp,bool iscalcurate)
	{
		if (frame %(60 *3)==0) {
			ranking_ = calcurateRanking(calcurateScore(score, maxcombo, nowhp));
			if (ranking_ != rankbuf) {
				t = 0.0f;
				endt_ = 0.0f;
			}
		}
		drawrank = Easing::getEasing[Easing::Linear](t,rankbuf,ranking_);
		Easing::tCount(t, 1.f);
		if (t >= 1.0f) {
			rankbuf = ranking_;
			Easing::tCount(endt_,0.3f);
		}
	
		frame++;
	}

	void Ranking::draw()
	{
		if (drawrank >= 100)return;
		Vec2f startsize=Vec2f(250, 200);
		Vec2f endsize=Vec2f(300, 250);
		size_.x = Easing::getEasing[Easing::Return](endt_, startsize.x, endsize.x);
		size_.y = Easing::getEasing[Easing::Return](endt_, startsize.y, endsize.y);
		Vec2f pos = Vec2f(env.getWindowWidth()*0.65, env.getWindowHeight()*0.9f);
		gl::pushModelView();
		gl::translate(pos);
		gl::color(ColorA(0,1,1,1));
		rank->enableAndBind();
		gl::drawSolidRect(Rectf(-size_ / 2, size_ / 2));
		rank->disable();

		if (drawrank >= 10) {
			num10[(drawrank / 10) - 1]->enableAndBind();
			gl::drawSolidRect(Rectf(-size_ / 2, size_ / 2));
			num10[(drawrank / 10) - 1]->disable();
		}

		num1[drawrank % 10]->enableAndBind();
		gl::drawSolidRect(Rectf(-size_ / 2, size_ / 2));
		num1[drawrank % 10]->disable();
		gl::popModelView();

	}

	void Ranking::roadtext()
	{
		std::ifstream scorefile(env.getAssetPath("savedata/score.txt").string());
		while (!scorefile.eof()) {
			int scorebuf;
			scorefile >> scorebuf;
			player_scores.push_back(scorebuf);
		}
	}

	int Ranking::calcurateRanking(int endscore)
	{
		for (int i = 0; i < player_scores.size(); i++) {
			if (player_scores[i] <endscore) {
				return i + 1;
			}
		}
		return player_scores.size() + 1;
	}

	int Ranking::calcurateScore(int score, int maxcombo, int nowhp)
	{
		int endscore = score + (5000 * maxcombo);
		endscore = endscore*(1 + (float(nowhp) / 100.f));
		return endscore;
	}


}
