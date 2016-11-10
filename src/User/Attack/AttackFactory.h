#pragma once

#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"

//! @file Attack
//! @brief ����Ő؂铮���������Ƃ���
//!�@�@�@�@ �J�n�ʒu�ƏI���ʒu�����߂Ďa���p�̍��W�ƓG�Ɠ������Ă��邩�ǂ����𑗂�܂�
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

	//������Ă��邩�ǂ���
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

	//���݂�flame�����܂�
	int frameCount;

	Line hitLine;

	//���[�V�����ɓ����Ă��邩�ǂ���
	bool isMotioning;

	bool isStarted;

	bool isEnded;

	//���W����������Ƃ�Ă�����
	//true -> �U�������ɓ����Ă����@false -> �����ɂ͂����Ă͂����Ȃ�
	bool canJudge;

};