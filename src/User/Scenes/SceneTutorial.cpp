# include "SceneTutorial.h"
# include "SceneResult.h"

# include "ZKOO.hpp"
# include "cinder/Rand.h"

# include "GlobalData.hpp"
# include "TutorialData.hpp"
# include "../Utilitys/GlobalDraw.h"

namespace User
{
	using namespace cinder;

	SceneTutorial::SceneTutorial()
	{
		// ユーマヨが管理するものを作成。
		cameraEyePosition = Vec3f(0, 0.7, -5);
		camera.lookAt(cameraEyePosition, Vec3f(0, 0.7, 0));
		camera.setWorldUp(Vec3f(0, 1, 0));
		camera.setPerspective(60.0F, env.getWindowAspectRatio(), 1.0F, 100.0F);
		fieldManager = std::make_shared<FieldManager>("JSON/TutorialField.json");
		enemyManager = std::make_shared<EnemyManager>(camera, fieldManager->FieldDataPath());
		enemyBulletManager = std::make_shared<EnemyBulletManager>();
		effectManager = std::make_shared<EffectManager>();
		UI = std::make_shared<Interface>();
		gameClearFrame = 60 * 3;
		timer.Off();
		damageColor = ColorA(1, 0, 0, 0);
		talk = std::make_shared<Talk>();

		// 野本が管理するものを作成。
		player = Player(100, 100);

		// 大ちゃんが管理するものを作成。
		mainbgm.push_back(&GData::FindAudio("BGM/mainbgm0.wav"));
		mainbgm[0]->Looping(true);
		mainbgm[0]->Gain(0.4);
		mainbgm[0]->Play();
		watercount = 0;
		fusuma = std::make_shared<Fusuma>();
		time = 0;/////////////////////////////////////////////////////////ここ！！
		ismpmax = false;
		mpmax = &GData::FindAudio("SE/mpmax.wav");
	}
	SceneTutorial::~SceneTutorial()
	{
		mainbgm[0]->Stop();
	}
	void SceneTutorial::resize()
	{
		camera.setAspectRatio(env.getWindowAspectRatio());
	}

	void SceneTutorial::UpdateDamage()
	{
		if (special.getSpecialType() == SpecialType::WATER) {
			player.TranseNowHp((watercount % 60) == 0);
			watercount++;
		}
		else { watercount = 0; }

		int playerHP = player.NowHp();

		// プレイヤーが何もしていないなら
		if (player.Command() == CommandType::NONE)
		{
			bool 無敵じゃない = (!(special.getSpecialType() == SpecialType::TREE));
			int damage = 0;
			damage += enemyManager->EnemyToPlayerDamage(camera) * 無敵じゃない;
			damage += enemyBulletManager->EnemyToPlayerDamage(camera) * 無敵じゃない;
			player.TranseNowHp(-damage);
		}

		// プレイヤーが攻撃状態なら
		if (player.Command() == CommandType::ATTACK)
		{
			int damage = 0;
			damage += enemyManager->EnemyToPlayerDamage(camera);
			damage += enemyBulletManager->EnemyToPlayerDamage(camera);
			player.TranseNowHp(-damage * !(special.getSpecialType() == SpecialType::TREE));
		}

		// プレイヤーがガード状態なら
		if (player.Command() == CommandType::GUARD)
		{
			int damage = 0;
			damage += enemyManager->EnemyToPlayerDamage(player.GuardLine(), camera);
			damage += enemyBulletManager->EnemyToPlayerDamage(player.GuardLine(), camera);
			player.TranseNowHp(-damage * !(special.getSpecialType() == SpecialType::TREE));
		}

		if (player.IsAttack())
		{
			float damagerate = (special.getSpecialType() == SpecialType::FIRE) ? 1.3f : 1.0f;
			player.TranseNowMp(enemyManager->PlayerToEnemyDamage(player.MakeLine(), camera, damagerate));
			player.TranseNowMp(enemyBulletManager->PlayerToEnemyDamage(player.MakeLine(), camera));
		}

		if (playerHP != player.NowHp())
		{
			damageColor.a = std::min(damageColor.a + 0.2, 0.6);
			timer.On();
			timer.Advance(60);
		}
	}
	void SceneTutorial::UpdatePlayerBegin()
	{
		player.Update();

		UpdatePlayerBehavior();
	}
	void SceneTutorial::UpdatePlayerBehavior()
	{
		player.AttackPhase();
	}
	void SceneTutorial::UpdatePlayerEnd()
	{
		if (player.IsAttack())
		{
			player.ShiftIsAttack();
		}
	}
	void SceneTutorial::UpdateColor()
	{
		damageColor.a = std::max(damageColor.a - 0.01F, 0.0F);
	}
	void SceneTutorial::UpdateScore()
	{
		UI->AddScore(enemyManager->ScoreRecovery());
		UI->AddScore(enemyBulletManager->ScoreRecovery());
	}
	void SceneTutorial::UpdateCombo()
	{
		if (player.IsAttack())
		{
			UI->update(0 != enemyManager->PlayerToEnemyAttackCheck(player.MakeLine(), camera));
		}
		else
		{
			UI->update(false);
		}
	}
	void SceneTutorial::UpdateDamageExpression()
	{
		timer.Update();

		if (timer.IsCount())
		{
			camera.lookAt(Vec3f(randFloat(-0.05F, 0.05F), randFloat(-0.05F, 0.05F), 0.0F) + cameraEyePosition, camera.getCenterOfInterestPoint());
		}

		if (timer.IsAction())
		{
			timer.Off();
			camera.lookAt(cameraEyePosition, camera.getCenterOfInterestPoint());
		}
	}
	void SceneTutorial::UpdateNextStage()
	{
		// エネミーが全滅したら、次のステージを準備。
		if (enemyManager->IsEmpty() && !fieldManager->IsLastField() && TRData::enemyKill.IsComplete())
		{
			fieldManager->End();
		}

		// ステージの移動が終了したら次のステージへ。
		if (!fieldManager->IsLastField() && fieldManager->IsChange())
		{
			fieldManager->ChangeField();
			enemyManager = std::make_shared<EnemyManager>(camera, fieldManager->FieldDataPath());
			enemyBulletManager = std::make_shared<EnemyBulletManager>();
		}

		// ラスボスを倒し終わったら、何らかのアクション後、次のシーンへ。
		if (enemyManager->IsEmpty() && fieldManager->IsLastField())
		{
			float gain = (static_cast<float>(gameClearFrame) / (60 * 3)) * 0.4F;
			mainbgm[0]->Gain(gain);
			gameClearFrame = std::max(gameClearFrame - 1, 0);
			camera.lookAt(Vec3f(randFloat(-0.05F, 0.05F), randFloat(-0.05F, 0.05F), 0.0F) + cameraEyePosition, camera.getCenterOfInterestPoint());
		}
	}
	void SceneTutorial::UpdateAllInstans()
	{
		// スペシャル選択時から、エフェクトが終わるまで全ての動作を止めます。
		if (!special.getIsSpecial())
		{
			fieldManager->Update();

			if (!TRData::IsStopUpdate())
			{
				enemyManager->update(camera);
				enemyBulletManager->BulletRegister(enemyManager->BulletsRecovery());
				enemyBulletManager->update();

				UpdateDamage();
				UpdateColor();
				UpdateScore();
				UpdateCombo();
			}

			effectManager->EffectRegister(enemyManager->EffectRecovery());
			effectManager->EffectRegister(enemyBulletManager->EffectRecovery());
			effectManager->Update();
		}
		/////////////////////////////////ここ！！
		if ((TRData::enemyKill.IsComplete()) && (!special.getIsSpecial()) && (!fieldManager->IsMoveing())) {
			time++;
		}
		if ((player.NowMp() == player.MaxMp()) && (!ismpmax)) {
			mpmax->Gain(0.5f);
			mpmax->Play();
			ismpmax = true;
		}
		if ((!(player.NowMp() == player.MaxMp())) && (ismpmax)) {
			ismpmax = false;
		}
		/////////////////////////////////
	}
	void SceneTutorial::UpdateSpecial()
	{
		// スペシャルは最初にアップデートします。
		// エネミーがいない場合はスペシャルを発動できないようにします。
		if (!enemyManager->IsEmpty())
		{
			special.update(player.NowMp() == player.MaxMp(), fieldManager->IsMoveing());///////////////////////////ここ！！
		}

		// プレイヤーのスペシャルエフェクト時にカメラを揺らします。
		if (special.isEffecting())
		{
			camera.lookAt(Vec3f(randFloat(-0.05F, 0.05F), randFloat(-0.05F, 0.05F), 0.0F) + cameraEyePosition, camera.getCenterOfInterestPoint());
			player.TranseNowMp(-1);
		}

		// エフェクト終了時、カメラを元通りにします。
		if (special.getEffectEnd())
		{
			camera.lookAt(cameraEyePosition, camera.getCenterOfInterestPoint());
		}

		// エフェクト終了時、全てのエネミー及び弾にダメージを与えます。
		if (special.getEffectEnd())
		{
			const float damagevalue = 10.0F;
			player.TranseNowMp(enemyManager->PlayerSpecialAttackToEnemyDamage(special.getspecialPower() * damagevalue, camera, special.getSpecialType()));
			player.TranseNowMp(enemyBulletManager->PlayerSpecialAttackToEnemyDamage());
		}
	}
	void SceneTutorial::UpdateDebugTutorialClear()
	{
		if (inputs.isPressKey(Key::KEY_LCTRL) && inputs.isPushKey(Key::KEY_1))
		{
			TRData::spawn.TutorialEnd();
		}
		if (inputs.isPressKey(Key::KEY_LCTRL) && inputs.isPushKey(Key::KEY_2))
		{
			TRData::attackCircle.TutorialEnd();
		}
		if (inputs.isPressKey(Key::KEY_LCTRL) && inputs.isPushKey(Key::KEY_3))
		{
			TRData::guard.TutorialEnd();
		}
		if (inputs.isPressKey(Key::KEY_LCTRL) && inputs.isPushKey(Key::KEY_4))
		{
			TRData::playerAttack.TutorialEnd();
		}
		if (inputs.isPressKey(Key::KEY_LCTRL) && inputs.isPushKey(Key::KEY_5))
		{
			TRData::special.TutorialEnd();
		}
	}
	void SceneTutorial::UpdateTutorialClear()
	{
		if (TRData::spawn.IsStopUpdate())
		{
			talk->Update();
			if (TRData::IsSerifTalked()) TRData::spawn.TutorialEnd();
		}
		if (TRData::attackCircle.IsStopUpdate())
		{
			talk->Update();
			if (TRData::IsSerifTalked()) TRData::attackCircle.TutorialEnd();
		}

		if (TRData::guard.IsStopUpdate())
		{
			static bool isSerifEnd = false;

			if (!isSerifEnd)
			{
				if (talk->IsAnyPushTouch())
				{
					talk->Update();
					if (TRData::IsSerifTalked())
					{
						TRData::talkString.clear();
						TRData::talkString.push_front(u8"両手を握り続けてみて！ そうするとガードになるわよ。");
						isSerifEnd = true;
					}
				}
				return;
			}

			if (TRData::guard.IsSuccessFull())
			{
				TRData::talkString.clear();
				TRData::talkString.push_front(u8"うまいじゃない！！");

				TRData::guard.TutorialEnd();
				isSerifEnd = false;
			}
			else if (player.Command() == CommandType::GUARD && enemyManager->EnemyToPlayerGuardCheck(player.GuardLine(), camera))
			{
				TRData::talkString.clear();
				TRData::talkString.push_front(u8"そうそうその調子。");

				TRData::guard.Update(true);
			}
			else if (player.Command() == CommandType::GUARD)
			{
				TRData::talkString.clear();
				TRData::talkString.push_front(u8"そのまま剣を赤い円の中に入れてみて！");

				TRData::guard.Update(false);
			}
			else
			{
				TRData::talkString.clear();
				TRData::talkString.push_front(u8"両手を握り続けてみて！ そうするとガードになるわよ。");

				TRData::guard.Update(false);
			}
		}

		if (TRData::playerAttack.IsStopUpdate())
		{
			static bool isSerifEnd = false;

			if (!isSerifEnd)
			{
				if (talk->IsAnyPushTouch())
				{
					talk->Update();
					if (TRData::IsSerifTalked())
					{
						TRData::talkString.clear();
						TRData::talkString.push_front(u8"手を握り続けて動かして！！");
						isSerifEnd = true;
					}
				}
				return;
			}

			// チュートリアル中でもダメージ計算を行います。
			UpdateDamage();
			enemyManager->update(camera);
			UpdateScore();
			UpdateCombo();

			if (enemyManager->IsEmpty())
			{
				TRData::playerAttack.TutorialEnd();
				isSerifEnd = false;
			}
			else if (player.IsAttack())
			{
				if (0 != enemyManager->PlayerToEnemyAttackCheck(player.MakeLine(), camera))
				{
					TRData::talkString.clear();
					TRData::talkString.push_front(u8"そうそうそんな感じよ！");
				}
			}
			else if (talk->IsAnyPushTouch())
			{
				TRData::talkString.clear();
				TRData::talkString.push_front(u8"そのまま手を動かして離すのよ！");
			}
			else if (talk->IsAnyPullTouch())
			{
				TRData::talkString.clear();
				TRData::talkString.push_front(u8"手を握り続けて動かして！！");
			}
		}

		if (TRData::enemyKill.IsStopUpdate())
		{
			talk->Update();
			if (TRData::IsSerifTalked()) TRData::enemyKill.TutorialEnd();
		}

		if (TRData::special.IsStopUpdate())
		{
			talk->Update();
			if (TRData::IsSerifTalked()) TRData::special.TutorialEnd();
		}
	}
	void SceneTutorial::update()
	{
		if (!special.getIsSpecial()) {
			UpdatePlayerBegin();
		}

		UpdateDebugTutorialClear();

		UpdateTutorialClear();

		UpdateSpecial();

		UpdateDamageExpression();

		UpdateAllInstans();

		UpdateNextStage();

		UpdatePlayerEnd();
	}
	void SceneTutorial::draw()
	{
		beginDrawMain();
		drawMain();
		endDrawMain();

		beginDrawUI();
		drawUI();
		endDrawUI();
	}
	void SceneTutorial::select()
	{
		if (inputs.isPressKey(Key::KEY_LCTRL) && inputs.isPushKey(Key::KEY_e))
		{
			create(new SceneResult(UI->Score(), UI->MaxComboNumber(), player.NowHp(), time / 60));///////ここ！！
			return;
		}

		if (player.NowHp() <= 0.0F)
		{
			float gain = (static_cast<float>(gameClearFrame) / (60 * 3)) * 0.4F;
			mainbgm[0]->Gain(gain);
			gameClearFrame = std::max(gameClearFrame - 1, 0);

			create(new SceneResult(UI->Score(), UI->MaxComboNumber(), player.NowHp(), time / 60));
			return;
		}

		if (gameClearFrame == 0)
		{
			fusuma->closeFusuma();

			if (fusuma->IsMoveFinished())
			{
				create(new SceneResult(UI->Score(), UI->MaxComboNumber(), player.NowHp(), time / 60));
				return;
			}
		}
	}
	void SceneTutorial::beginDrawMain()
	{
		gl::pushMatrices();
		gl::enable(GL_TEXTURE_2D);
		gl::enable(GL_CULL_FACE);

		gl::enableAlphaBlending();

		gl::enableDepthWrite();
		gl::enableDepthRead();

		//gl::enable( GL_NORMALIZE );
		//gl::enable( GL_LIGHTING );
		//glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Color::black( ) );
		//glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

		gl::setViewport(env.getWindowBounds());
		gl::setMatrices(camera);
	}
	void SceneTutorial::drawMain()
	{
		if (special.isMinigame()) return;

		fieldManager->Draw(camera);
		enemyManager->draw(camera);
		enemyBulletManager->draw(camera);

		GlobalDraw::Draw();
	}
	void SceneTutorial::endDrawMain()
	{
		gl::popMatrices();
	}
	void SceneTutorial::beginDrawUI()
	{
		gl::pushMatrices();
		gl::enable(GL_TEXTURE_2D);
		gl::disable(GL_CULL_FACE);
		gl::disable(GL_NORMALIZE);
		gl::disable(GL_LIGHTING);
		gl::enableAlphaBlending();
		gl::disableDepthRead();
		gl::disableDepthWrite();

		gl::setViewport(env.getWindowBounds());
		gl::setMatricesWindow(env.getWindowSize());
		gl::color(Color::white());
	}
	void SceneTutorial::drawUI()
	{
		if (!special.isMinigame())
		{
			player.Draw();

			enemyManager->DrawAttackCircle(camera);

			enemyBulletManager->DrawBulletCircle(camera);

			effectManager->Draw();

			UI->draw(player.NormalizedMp(), player.NormalizedHp(),
				(player.NowMp() == player.MaxMp()) && (special.getSpecialType() == SpecialType::NOTSELECTED),
				int(special.getSpecialType()));

			talk->Draw(Vec2f(0, env.getWindowHeight() - 300));
		}

		special.draw();

		TRData::guard.Draw();

		gl::color(damageColor);
		gl::drawSolidRect(Rectf(Vec2f::zero(), env.getWindowSize()));

		fusuma->drawFusuma();
	}
	void SceneTutorial::endDrawUI()
	{
		gl::popMatrices();
	}
}