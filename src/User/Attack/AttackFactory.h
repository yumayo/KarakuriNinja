#pragma once

#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"

//! @file Attack
//! @brief 動作で切る動きをしたときに
//!　　　　 開始位置と終了位置を決めて斬撃用の座標と敵と当たっているかどうかを送ります
//! @note
//!              
//! @date 2016-07-06
//! @author Taka Nomoto

class AttackFactory
{
public:

	AttackFactory() : frameCount(0), canJudge(false), isStarted(false), isEnded(false), isMotioning(false)
	{

	}

	~AttackFactory()
	{

	}

	Line HitLine()
	{
		return hitLine;
	}

	bool StartingAttack()
	{
		return frameCount == START_ATTACK_TIME;
	}

	bool EndingAttack()
	{
		return frameCount == END_ATTACK_TIME;
	}

	bool NotMotioning()
	{
		return false == isMotioning;
	}

	//押されているかどうか
	void Update();

	void  Reset()
	{
		isMotioning = false;
		frameCount = 0;
		isStarted = false;
		isEnded = false;
	}

	bool IsMotioning()
	{
		return isMotioning;
	}

	void ShiftIsMotioning()
	{
		isMotioning = !isMotioning;
	}

	void SetStartPos(ci::Vec2f pos_)
	{
		hitLine.startPos = pos_;
	}

	void SetEndPos(ci::Vec2f pos_)
	{
		hitLine.endPos = pos_;
	}

	bool IsStarted()
	{
		return isStarted;
	}

	void  ShiftIsStarted()
	{
		isStarted = !isStarted;
	}

	bool IsEnded()
	{
		return isEnded;
	}

	void ShiftIsEnded()
	{
		isEnded = !isEnded;
	}

	bool CanJudge()
	{
		return canJudge;
	}

	void ShiftCanJudge()
	{
		canJudge = !canJudge;
	}
private:

	//現在のflameを見ます
	int frameCount;

	Line hitLine;

	//モーションに入っているかどうか
	bool isMotioning;

	bool isStarted;

	bool isEnded;

	//座標がしっかりとれていたら
	//true -> 攻撃処理に入っていい　false -> 処理にはいってはいけない
	bool canJudge;

};