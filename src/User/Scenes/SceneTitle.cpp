# include "SceneTitle.h"
# include "SceneTutorial.h"
# include "SceneGame.h"

#include "GlobalData.hpp"

using namespace ci;

namespace User
{
	SceneTitle::SceneTitle()
		: font(u8"メイリオ", 80)
		, logoAlpha(0.0f)
		, logoAlphaSpeed(0.0125f)
		, isEnd(false)
		, endt_(0.0f)
		, isblackout(false)
	{
		startButtonPosition = env.getWindowCenter() + Vec2f(0, 100);
		bgm = &GData::FindAudio("BGM/mainbgm1.wav");
		bgm->Looping(true);
		bgm->Gain(0.5f);
		bgm->Play();
	}

	SceneTitle::~SceneTitle()
	{
	}

	void SceneTitle::resize()
	{
		startButtonPosition = env.getWindowCenter() + Vec2f(0, 100);
	}

	void SceneTitle::update()
	{
		slashInput.Begin();

		UpdateLogoAlpha();

		// スラッシュとの当たり判定を取るには、円の中心ポジションと半径を入れます。
		//if (slashInput.IsHitCircle(startButtonPosition, 125)) isEnd = true;
		if (!isblackout) {
			isblackout = isStartTouch();
		}
		if (isblackout) {
			Easing::tCount(endt_,2.0f);
		}
		if (endt_ >= 1.0f)isEnd = true;
		slashInput.End();
	}
	void SceneTitle::draw()
	{
		beginDrawUI();
		drawUI();
		endDrawUI();
	}
	void SceneTitle::select()
	{
		if (isEnd == false) return;
		//create( new SceneGame( ) );
		bgm->Stop();
		create(new SceneTutorial());
	}

	void SceneTitle::UpdateLogoAlpha()
	{
		logoAlpha += logoAlphaSpeed;

		if (logoAlpha <= 0 || logoAlpha >= 1)
			logoAlphaSpeed *= -1;
	}

	void SceneTitle::beginDrawUI()
	{
		ci::gl::enable(GL_TEXTURE_2D);
		ci::gl::disable(GL_CULL_FACE);
		ci::gl::disable(GL_NORMALIZE);
		ci::gl::disable(GL_LIGHTING);
		ci::gl::enableAlphaBlending();
		ci::gl::disableDepthRead();
		ci::gl::disableDepthWrite();

		ci::gl::setViewport(env.getWindowBounds());
		ci::gl::setMatricesWindow(env.getWindowSize());
		ci::gl::color(Color::white());
	}
	void SceneTitle::drawUI()
	{
		drawTexture(ci::Vec2f(0, 0), env.getWindowSize(), GData::FindTexture("Textures/Title/title.png"), ci::ColorA::white());

		// drawTextureでアルファブレンディングをOFFにしているので。
		ci::gl::enableAlphaBlending();

		// スタートボタン
		gl::color(ci::ColorA(1, 1, 1, logoAlpha));
		gl::draw(GData::FindTexture("Textures/Title/start.jpg"), Rectf(startButtonPosition.x - 125.0F, startButtonPosition.y - 125.0F,
			startButtonPosition.x + 125.0F, startButtonPosition.y + 125.0F));
		font.Draw(u8"スタート", startButtonPosition + Vec2f(0, -80 / 2), Color::white(), Fonts::Mode::CENTERUP);

		slashInput.Draw();
		drawfade();
	}
	void SceneTitle::endDrawUI()
	{

	}
	bool SceneTitle::isStartTouch()
	{
		auto touch = inputs.touch();
		auto ids = inputs.GetTouchHandleIDs();

		for (auto id : ids)
		{

			//////とりあえずこれでプレイヤーアイコンの近くをプッシュするとスペシャルモードに入れるようにしておきました
			if (inputs.isPushTouch(id, touch))
			{
				Vec2f iconpos = startButtonPosition;
				float iconsize = 128.f;
				float kyori = (iconpos.x - touch.getPos().x)*(iconpos.x - touch.getPos().x) + (iconpos.y - touch.getPos().y)*(iconpos.y - touch.getPos().y);
				if (kyori < (iconsize*iconsize)) {
					return true;
				}
			}
			//////
		}
		return false;
	}
	void SceneTitle::drawfade()
	{
		gl::pushModelView();
		gl::translate(env.getWindowSize()/2);
		gl::color(ColorA(0,0,0,endt_));
		gl::drawSolidRect(Rectf(-env.getWindowSize() / 2, env.getWindowSize() / 2));
		gl::popModelView();
	}
}