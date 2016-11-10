#pragma once

#include "../Utilitys/Nomoto.h"
#include "../SlashEffect/SlashEffect.h"
#include "../Deffence/Deffence.h"
#include "../Attack/AttackFactory.h"
#include"../Special/Special.h"
//! @file Player
//! @brief Playerを管理するクラスです
//! @note 
//!      Playerの基礎情報は随時追加します
//!      描画は基本的に外のクラスにわたしてやります
//!      Hpの最大値 : 100
//!      Mpの最大値 : 100
//! @date 2016-07 - 08
//! @author Taka Nomoto
//!
//! @date 2016-07 - 19
//! Attack追加・Guard追加

const int FIRE_MAGIC_POINT = 100;
const int WATER_MAGIC_POINT = 100;
const int WEED_MAGIC_POINT = 100;

const int MIN_HP = 0;
const int MIN_MP = 0;

class Player
{
public:
	Player() : isAttack(false),isGuard(false),nowCommand(NONE)
	{

	}

	Player(int hp_, int mp_) : isAttack(false), isGuard(false), nowCommand(NONE)
	{
		nowHp = maxHp = hp_;
		maxMp = mp_;
        nowMp = MIN_MP + 1;
		nowCommand = CommandType::NONE;
	}

	void Touch();

	void SetAttackMotionOfTouch(cinder::app::TouchEvent::Touch touch, uint32_t id);

	void MakeAttackMotionOfTouch(cinder::app::TouchEvent::Touch touch, uint32_t id);

	void Zkoo();

	void UpdateDeffenceOfZkoo();

	void SetAttackMotionOfZkoo();
	
	void MakeAttackMotionOfZkoo();

	void AttackPhase();

	void UpdateDeffenceOfTouch();

	void ShiftIsAttack() { isAttack = !isAttack; }

	bool IsAttack() { return isAttack; }

	Line MakeLine() { return line; }

	Line GuardLine() { return deffence.GuardLine(); }

	void Draw();

	int NowHp() { return nowHp; }

	void TranseNowHp(int value_);

	int MaxHp() { return maxHp; }
	void SetMaxHp(int maxHp_) { maxHp = maxHp_; }

	int NowMp() { return nowMp; }
	void TranseNowMp(int value_);

	int MaxMp() { return maxMp; }
	void SetMaxMp(int maxMp_) { maxMp = maxMp_; }
	
	float NormalizedHp() { return static_cast<float>(nowHp) / static_cast<float>(maxHp); }
	
	float NormalizedMp() { return static_cast<float>(nowMp) / static_cast<float>(maxMp); }

	CommandType Command() { return nowCommand; }


private:
	int nowHp;
	int maxHp;

	int nowMp;
	int maxMp;

	Slash slashEffect;
	AttackFactory attackTask;
	Line line;
	bool isAttack;

	Deffence deffence;
	bool isGuard;
	CommandType nowCommand;
};
