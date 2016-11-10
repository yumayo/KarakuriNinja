# include "SceneTitle.h"
# include "SceneTutorial.h"

#include "GlobalData.hpp"

using namespace ci;

namespace User
{
    SceneTitle::SceneTitle( )
        : isEnd( false )
        , endt_( 0.0f )
        , isblackout( false )
    {
        bgm = &GData::FindAudio( "BGM/mainbgm1.wav" );
		cross_se = &GData::FindAudio("SE/titlecross.wav");
		roadTexture();

		for (int i = 0;i < TitleEasing::TitleMAX;i++) {
			t_[i] = 0.0f;
		}
		for (int i = 0;i < SE_END::SE_MAX;i++) {
			se_ends[i] = false;
		}
		pushKurikoShuriken();
		pushBossShuriken();
        bgm->Looping( true );
        bgm->Gain( 0.5f );
        bgm->Play( );
		enablenext = false;
		iconangle = 0.0f;
		kunaiangle = 0.0f;
    }

    SceneTitle::~SceneTitle( )
    {
        bgm->Stop( );
    }

    void SceneTitle::resize( )
    {

    }

    void SceneTitle::update( )
    {
        // スラッシュとの当たり判定を取るには、円の中心ポジションと半径を入れます。
        //if (slashInput.IsHitCircle(startButtonPosition, 125)) isEnd = true;
        if ( !isblackout ) {
            isblackout = isStartTouch( );
        }
		skip();
		if (countend(t_[TitleEasing::TitleMAX - 1]))
			enablenext = true;
        if ( isblackout ) {
            Easing::tCount( endt_, 2.0f );
        }
        if ( endt_ >= 1.0f )isEnd = true;
		if (countend(t_[ONEDELAY])) {
			PlaySE(cross_se,se_ends[SE_END::SE_CROSS]);
		}
		for (int i = 0;i < kuriko_shuriken.size();i++) {
			kuriko_shuriken[i].update(15.f);
		}
		if (countend(t_[KURIKODELAY])) {
			for (int i = 0;i < boss_kunai.size();i++) {
				boss_kunai[i].update(-15.f);
			}
		}
    }
    void SceneTitle::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneTitle::select( )
    {
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_g ) )
        {
            create( new SceneTutorial( ) );
            return;
        }

        if ( isEnd )
        {
            create( new SceneTutorial( ) );
            return;
        }
    }

    void SceneTitle::beginDrawUI( )
    {
        ci::gl::enable( GL_TEXTURE_2D );
        ci::gl::disable( GL_CULL_FACE );
        ci::gl::disable( GL_NORMALIZE );
        ci::gl::disable( GL_LIGHTING );
        ci::gl::enableAlphaBlending( );
        ci::gl::disableDepthRead( );
        ci::gl::disableDepthWrite( );

        ci::gl::setViewport( env.getWindowBounds( ) );
        ci::gl::setMatricesWindow( env.getWindowSize( ) );
        ci::gl::color( Color::white( ) );
    }
    void SceneTitle::drawUI( )
    {
        //drawTexture( ci::Vec2f( 0, 0 ), env.getWindowSize( ), GData::FindTexture( "Textures/Title/title.png" ), ci::ColorA::white( ) );

        // drawTextureでアルファブレンディングをOFFにしているので。
        ci::gl::enableAlphaBlending( );

		drawInKurio();
		drawInBoss();
		crossdraw();
		drawSlash();
		drawBackground();
		drawMainKuriko();
		drawLogo();
		drawKunai();
		drawStartIcon();
		drawFlash();
        drawfade( );
    }
    void SceneTitle::endDrawUI( )
    {
		
    }

	void SceneTitle::skip()
	{
		auto touch = inputs.touch();
		auto ids = inputs.GetTouchHandleIDs();

		for (auto id : ids)
		{
			//////とりあえずこれでプレイヤーアイコンの近くをプッシュするとスペシャルモードに入れるようにしておきました
			if (inputs.isPushTouch(id, touch))
			{
				enablenext = true;
				for (int i = 0;i < TitleEasing::TitleMAX;i++) {
					t_[i] = true;
				}
			}
			//////
		}
	}
	void SceneTitle::drawKunai()
	{
		if (!countend(t_[LOGOSLIDE]))return;
		Vec2f endpos = env.getWindowSize() / 2;
		Vec2f size = env.getWindowSize();
		gl::pushModelView();
		gl::translate(endpos + Vec2f(0, 15 * sin(kunaiangle)));
		textures[T_MAINKUNAI]->enableAndBind();
		gl::color(ColorA(1, 1, 1, 1));
		gl::drawSolidRect(Rectf(-size / 2.f, size / 2.f));
		textures[T_MAINKUNAI]->disable();
		gl::popModelView();
		kunaiangle += 0.05;
		
		gl::pushModelView();
		gl::translate(endpos + Vec2f(0, 60 * sin(kunaiangle)));
		textures[T_SUBKUNAI]->enableAndBind();
		gl::color(ColorA(1, 1, 1, 1));
		gl::drawSolidRect(Rectf(-size / 2.f, size / 2.f));
		textures[T_SUBKUNAI]->disable();
		gl::popModelView();
		kunaiangle += 0.05;
	}
    bool SceneTitle::isStartTouch( )
    {
		if (!enablenext)return false;
        auto touch = inputs.touch( );
        auto ids = inputs.GetTouchHandleIDs( );

        for ( auto id : ids )
        {
            //////とりあえずこれでプレイヤーアイコンの近くをプッシュするとスペシャルモードに入れるようにしておきました
            if ( inputs.isPushTouch( id, touch ) )
            {
                return true;
            }
            //////
        }
        return false;
    }
    void SceneTitle::drawfade( )
    {
        gl::pushModelView( );
        gl::translate( env.getWindowSize( ) / 2 );
        gl::color( ColorA( 0, 0, 0, endt_ ) );
        gl::drawSolidRect( Rectf( -env.getWindowSize( ) / 2, env.getWindowSize( ) / 2 ) );
        gl::popModelView( );
    }
	void SceneTitle::drawInKurio()
	{
		if (countend(t_[TitleEasing::ONEDELAY]))return;
		float posx = env.getWindowWidth() / 2 + env.getWindowWidth() / 6.f;
		Easingdraw(t_[KURIKOBACK], 0.6f, Vec2f(posx, -1.5f*env.getWindowHeight() / 2),
			Vec2f(env.getWindowWidth() / 2 + env.getWindowWidth() / 6.f, env.getWindowHeight() / 2), Vec2f(env.getWindowWidth() / 3, env.getWindowHeight()),
			0.f, textures[TexType::T_KURIKOBACK],
			ColorA(1, 1, 1, 1), Easing::EasingType::Linear, Easing::EasingType::CubicIn);

		if (countend(t_[KURIKOBACK]))
			Easing::tCount(t_[KURIKOBACKDELAY],0.4f);
		for (int i = 0;i < kuriko_shuriken.size();i++) {
			kuriko_shuriken[i].draw(posx);
		}
		if (countend(t_[KURIKOBACKDELAY])) {
			if (!countend(t_[KURIKO])) {
				Easingdraw(t_[KURIKO], 0.6f, Vec2f(posx, -1.5f*env.getWindowHeight() / 2),
					Vec2f(posx, env.getWindowHeight() / 2 - 150), Vec2f(env.getWindowWidth() / 2, env.getWindowHeight()),
					0.f, textures[TexType::T_KURIKO],
					ColorA(1, 1, 1, 1), Easing::EasingType::Linear, Easing::EasingType::CubicIn);
			}
			else {
				Easingdraw(t_[KURIKODELAY], 1.3f, Vec2f(posx, env.getWindowHeight() / 2 - 150),
					Vec2f(posx, env.getWindowHeight() / 2), Vec2f(env.getWindowWidth() / 2, env.getWindowHeight()),
					0.f, textures[TexType::T_KURIKO],
					ColorA(1, 1, 1, 1), Easing::EasingType::Linear, Easing::EasingType::Linear);
			}

		}	
	}
	void SceneTitle::drawInBoss()
	{
		if (countend(t_[TitleEasing::ONEDELAY]))return;
		if (!countend(t_[KURIKODELAY]))return;
		float posx = env.getWindowWidth() / 2 - env.getWindowWidth() / 6.f;
		Easingdraw(t_[BOSSBACK], 0.6f, Vec2f(posx, 1.75f*env.getWindowHeight()),
			Vec2f(posx, env.getWindowHeight() / 2), Vec2f(env.getWindowWidth() / 3, env.getWindowHeight()),
			0.f, textures[TexType::T_BOSSBACK],
			ColorA(1, 1, 1, 1), Easing::EasingType::Linear, Easing::EasingType::CubicIn);

		for (int i = 0;i < boss_kunai.size();i++) {
			boss_kunai[i].draw(posx);
		}

		if (countend(t_[BOSSBACK]))
			Easing::tCount(t_[BOSSBACKDELAY],1.0f);
		if (countend(t_[BOSSBACKDELAY])) {
			if (!countend(t_[BOSS])) {
				Easingdraw(t_[BOSS], 0.6f, Vec2f(env.getWindowWidth() / 2 - env.getWindowWidth() / 6.f, 1.75f*env.getWindowHeight()),
					Vec2f(env.getWindowWidth() / 2 - env.getWindowWidth() / 6.f, env.getWindowHeight() / 2 + 150), Vec2f(env.getWindowWidth() / 2, env.getWindowHeight()),
					0.f, textures[TexType::T_BOSS],
					ColorA(1, 1, 1, 1), Easing::EasingType::Linear, Easing::EasingType::CubicIn);
			}
			else {
				Easingdraw(t_[BOSSDELAY], 1.3f, Vec2f(env.getWindowWidth() / 2 - env.getWindowWidth() / 6.f, env.getWindowHeight() / 2 + 150),
					Vec2f(env.getWindowWidth() / 2 - env.getWindowWidth() / 6.f, env.getWindowHeight() / 2), Vec2f(env.getWindowWidth() / 2, env.getWindowHeight()),
					0.f, textures[TexType::T_BOSS],
					ColorA(1, 1, 1, 1), Easing::EasingType::Linear, Easing::EasingType::Linear);
			}
		}
		if (countend(t_[BOSSDELAY]))
			Easing::tCount(t_[ONEDELAY],0.55f);
	}

	void SceneTitle::drawTex(cinder::Vec2f pos, cinder::Vec2f size, float rotate, cinder::gl::Texture * tex, cinder::ColorA color)
	{
		gl::pushModelView();
		gl::translate(pos);
		gl::rotate(rotate);
		gl::scale(size);
		gl::color(color);
		tex->enableAndBind();
		gl::drawSolidRect(Rectf(-Vec2f(0.5f,0.5f), Vec2f(0.5f, 0.5f)));
		tex->disable();
		gl::popModelView();
	}

	void SceneTitle::Easingdraw(float & t, float t_time, cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size, float rotate, cinder::gl::Texture * tex, cinder::ColorA color, Easing::EasingType x_type, Easing::EasingType y_type, bool count)
	{
		if(count)
		Easing::tCount(t, t_time);
		Vec2f pos;
		pos.x = Easing::getEasing[x_type](t, startpos.x, endpos.x);
		pos.y = Easing::getEasing[y_type](t, startpos.y, endpos.y);
		drawTex(pos, size, rotate, tex, color);
	}

	void SceneTitle::crossdraw()
	{
		if (!countend(t_[TitleEasing::ONEDELAY]))return;

			float posx = env.getWindowWidth() / 2 + env.getWindowWidth() / 6.f;
			float endposx = -1.5f * env.getWindowWidth() / 2;
			float bossposx = env.getWindowWidth() / 2 - env.getWindowWidth() / 6.f;
			float bossendposx = 1.75f * env.getWindowWidth();
			float posy = env.getWindowHeight() / 2;
			float kunaipos;
			float shurikenpos;
			Easing::EasingType type = Easing::EasingType::CubicInOut;
			kunaipos = Easing::getEasing[type](t_[CROSS],posx,endposx);
			shurikenpos = Easing::getEasing[type](t_[CROSS], bossposx, bossendposx);
			Easingdraw(t_[CROSS], 1.0f, Vec2f(posx, posy),
				Vec2f(endposx, posy), Vec2f(env.getWindowWidth() / 3, env.getWindowHeight()),
				0.f, textures[TexType::T_KURIKOBACK],
				ColorA(1, 1, 1, 1), type, Easing::EasingType::Linear);
			Easingdraw(t_[CROSS], 0.7f, Vec2f(bossposx, posy),
				Vec2f(bossendposx, posy), Vec2f(env.getWindowWidth() / 3, env.getWindowHeight()),
				0.f, textures[TexType::T_BOSSBACK],
				ColorA(1, 1, 1, 1), type, Easing::EasingType::Linear, false);

			for (int i = 0;i < kuriko_shuriken.size();i++) {
				kuriko_shuriken[i].draw(shurikenpos);
			}
			for (int i = 0;i < boss_kunai.size();i++) {
				boss_kunai[i].draw(shurikenpos);
			}

			Easingdraw(t_[CROSS], 0.7f, Vec2f(posx, posy),
				Vec2f(endposx, posy), Vec2f(env.getWindowWidth() / 2, env.getWindowHeight()),
				0.f, textures[TexType::T_KURIKO],
				ColorA(1, 1, 1, 1), type, Easing::EasingType::Linear,false);
			Easingdraw(t_[CROSS], 0.7f, Vec2f(bossposx, posy),
				Vec2f(bossendposx, posy), Vec2f(env.getWindowWidth() / 2, env.getWindowHeight()),
				0.f, textures[TexType::T_BOSS],
				ColorA(1, 1, 1, 1), type, Easing::EasingType::Linear, false);
	}

	void SceneTitle::PlaySE(Audio * se, bool & endflag)
	{
		if (!endflag) {
			se->Play();
			endflag = true;
		}
	}

	void SceneTitle::pushKurikoShuriken()
	{
		for (int i = 0;i < 30;i++) {
			Vec2f pos = Vec2f(randFloat(-150, 150), (i*-350) - 600);
			Vec2f size = Vec2f(200, 200);
			kuriko_shuriken.push_back(TitleCircle(pos, size, textures[T_KUNAI]));
		}
	}

	void SceneTitle::pushBossShuriken()
	{
		for (int i = 0;i < 30;i++) {
			Vec2f pos = Vec2f(randFloat(-150, 150), env.getWindowHeight()+(i*+350) + 600);
			Vec2f size = Vec2f(200, 200);
			 boss_kunai.push_back(TitleCircle(pos, size, textures[T_SHURIKEN]));
		}
	}

	void SceneTitle::drawSlash()
	{
		if (!countend(t_[ONEDELAY]))return;
		if (countend(t_[CROSS]))return;

		float alfa = Easing::getEasing[Easing::CubicIn](t_[CROSS], 1.0f, 0.f);
		Vec2f size(env.getWindowWidth()*1.3, env.getWindowHeight() / 4.f);

		gl::pushModelView();
		gl::translate(env.getWindowSize()/2);
		gl::rotate(-45);
		gl::color(ColorA(1,0,0,alfa));
		textures[T_SLASH]->enableAndBind();
		gl::drawSolidRect(Rectf(-size/2,size/2));
		textures[T_SLASH]->disable();
		gl::popModelView();

		gl::pushModelView();
		gl::translate(env.getWindowSize() / 2);
		gl::rotate(45);
		gl::color(ColorA(1, 1, 1, alfa));
		textures[T_SLASH]->enableAndBind();
		gl::drawSolidRect(Rectf(-size / 2, size / 2));
		textures[T_SLASH]->disable();
		gl::popModelView();
	}
	void SceneTitle::drawBackground()
	{
		if (!countend(t_[CROSS])) return;
		Easing::tCount(t_[UP],3.0f);
		float alfa = Easing::getEasing[Easing::QuadIn](t_[UP], 0.f, 1.f);
		Vec2f size;
		Vec2f endsize = env.getWindowSize();
		Vec2f startsize = endsize*10.0f;
		size.x = Easing::getEasing[Easing::CubicIn](t_[UP], startsize.x, endsize.x);
		size.y = Easing::getEasing[Easing::CubicIn](t_[UP], startsize.y, endsize.y);
		gl::pushModelView();
		gl::translate(env.getWindowSize() / 2);
		gl::color(ColorA(1, 1, 1, alfa));
		textures[T_BACKGROUND]->enableAndBind();
		gl::drawSolidRect(Rectf(-size / 2, size / 2));
		textures[T_BACKGROUND]->disable();
		gl::popModelView();
	}
	void SceneTitle::drawFlash()
	{
		if (!countend(t_[LOGOSLIDE])) return;
		if (countend(t_[FLASH])) return;
		Easing::tCount(t_[FLASH], 0.2f);
		gl::pushModelView();
		gl::translate(env.getWindowSize() / 2);
		gl::color(ColorA(1, 1, 1, Easing::getEasing[Easing::Return](t_[FLASH], 0, 1)));
		gl::drawSolidRect(Rectf(-env.getWindowSize() / 2, env.getWindowSize() / 2));
		gl::popModelView();
	}

	void SceneTitle::drawLogo()
	{
		if (!countend(t_[UP]))return;
		Vec2f startpos = Vec2f(-env.getWindowWidth()/2.0f, env.getWindowHeight() / 1.45f);
		Vec2f endpos = Vec2f(env.getWindowWidth() / 3.6f, env.getWindowHeight() / 1.45f);
		Vec2f size;
		size.x = env.getWindowWidth()/ 2.7f;
		size.y = size.x*((float(textures[T_LOGO]->getHeight()) / float(textures[T_LOGO]->getWidth())));
		int num = 3;
		if (countend(t_[TitleEasing::LOGOSLIDE])) {
			num = 1;
		}
			for (int i = num - 1;i >= 0;i--) {
				Vec2f pos;
				pos.x = Easing::getEasing[Easing::Linear](t_[TitleEasing::LOGOSLIDE] - 2*i*(1.0f / (60.0f*1.0f)), startpos.x, endpos.x);
				pos.y = Easing::getEasing[Easing::Linear](t_[TitleEasing::LOGOSLIDE] - i*(1.0f / (60.0f*1.0f)), startpos.y, endpos.y);
				gl::pushModelView();
				gl::translate(pos);
				textures[T_LOGO]->enableAndBind();
				float color = 1 - float(i) / float(num);
				gl::color(ColorA(color, color,color,color));
				gl::drawSolidRect(Rectf(-size / 2.f, size / 2.f));
				textures[T_LOGO]->disable();
				gl::popModelView();
			}

	
		Easing::tCount(t_[TitleEasing::LOGOSLIDE],1.0f);
	}
	void SceneTitle::drawMainKuriko()
	{
		if (!countend(t_[UP]))return;
		Vec2f startpos = Vec2f(1.5f*env.getWindowWidth(), env.getWindowHeight() / 2.f);
		Vec2f endpos = env.getWindowSize() / 2;
		Vec2f size = env.getWindowSize();
		int num = 3;
		if (countend(t_[TitleEasing::LOGOSLIDE])) {
			num = 1;
		}
		for (int i = num-1;i >=0;i--) {
			Vec2f pos;
			pos.x = Easing::getEasing[Easing::Linear](t_[TitleEasing::LOGOSLIDE] - 2 * i*(1.0f / (60.0f*1.0f)), startpos.x, endpos.x);
			pos.y = Easing::getEasing[Easing::Linear](t_[TitleEasing::LOGOSLIDE] - i*(1.0f / (60.0f*1.0f)), startpos.y, endpos.y);
			gl::pushModelView();
			gl::translate(pos);
			textures[T_MAINKURIKO]->enableAndBind();
			float color = 1 - float(i) / float(num);
			gl::color(ColorA(color, color, 1, color));
			gl::drawSolidRect(Rectf(-size / 2.f, size / 2.f));
			textures[T_MAINKURIKO]->disable();
			gl::popModelView();
		}
	}
	void SceneTitle::drawStartIcon()
	{
		if (!countend(t_[FLASH]))return;
		Vec2f pos=Vec2f( env.getWindowWidth( ) / 3.6f, env.getWindowHeight( ) / 1.1f );
		Vec2f size;
		size.x = env.getWindowWidth()/5.f;
		size.y = size.x*((float(textures[T_START]->getHeight()) / float(textures[T_START]->getWidth())));
		gl::pushModelView();
		gl::translate(pos);
		iconangle += 0.05f;
		gl::color(ColorA(1,1,1,0.5f+0.5*sin(iconangle)));
		textures[T_START]->enableAndBind();
		gl::drawSolidRect(Rectf(-size / 2, size / 2));
		textures[T_START]->disable();
		gl::popModelView();
	}
	void SceneTitle::roadTexture()
	{
		std::string str[TexType::TEXMAX];
		str[T_KURIKO] = "titlekuriko";
		str[T_BOSS] = "titleboss";
		str[T_KURIKOBACK] = "kurikoback";
		str[T_BOSSBACK] = "bossback";
		str[T_SHURIKEN] = "shuriken";
		str[T_KUNAI] = "titleguard";
		str[T_SLASH] = "titleslash";
		str[T_BACKGROUND] = "title";
		str[T_LOGO] = "logo";
		str[T_MAINKURIKO] = "mainkuriko";
		str[T_START] = "start";
		str[T_MAINKUNAI] = "mainkunai";
		str[T_SUBKUNAI] = "titlekunai";
		for (int i = 0;i < TexType::TEXMAX;i++) {
			textures[i] = &GData::FindTexture("Textures/Title/" + str[i] + ".png");
		}
	}
}