# include "Player.h"
# include "ZKOO.hpp"

void Player::TranseNowHp(int value_)
{
	nowHp += value_;
	nowHp = Clamp(nowHp, MIN_HP, maxHp);
}

void Player::TranseNowMp(int value_)
{
	nowMp += value_;
	nowMp = Clamp(nowMp, MIN_MP, maxMp);
}

void Player::Reset( )
{
    slashEffect = Slash();
    attackTask = AttackFactory();
    line = Line();
    isAttack = false;

    deffence = Deffence();
    isGuard = false;
    nowCommand = NONE;
}

void Player::Zkoo()
{
	if (zkoo.IsHandUsing())
	{
		if (zkoo.Left()->IsTracking() && zkoo.Left()->IsTouching() && zkoo.Right()->IsTracking() && zkoo.Right()->IsTouching())
		{
			UpdateDeffenceOfZkoo();
		}

		else if (zkoo.Right()->IsTracking() && zkoo.Right()->IsTouching())
		{
			SetAttackMotionOfZkoo();
		}
		//Slash�̏I�����ԂɂȂ�����E��̈ʒu��`��̍Ō�̈ʒu�ɓ���܂�
		if (!zkoo.Right()->IsTouching())
		{
			MakeAttackMotionOfZkoo();
		}
	}
}

void Player::UpdateDeffenceOfZkoo()
{
	if (attackTask.IsMotioning() == true)
	{
		attackTask.Reset();
	}

	if (deffence.IsActive() == false)
	{
		deffence.ShiftIsActive();
		nowCommand = CommandType::GUARD;
	}

	else if (deffence.IsActive() == true)
	{
		deffence.Update(zkoo.Right()->Position(), zkoo.Left()->Position());
	}
}

void Player::SetAttackMotionOfZkoo()
{


	if (deffence.IsActive() == true)
	{
		deffence.ShiftIsActive();
	}
	attackTask.Update();

	//Motion�ɓ����Ă��Ȃ��ꍇ
	if (attackTask.IsMotioning() == false && slashEffect.Active() == false)
	{
		attackTask.ShiftIsMotioning();
	}

	//Slash�̊J�n���ԂɂȂ�����E��̈ʒu��`��̍ŏ��̈ʒu�ɓ���܂�
	if (attackTask.IsStarted() == true)
	{
		attackTask.SetStartPos(zkoo.Right()->Position());
		attackTask.ShiftIsStarted();
	}
}

void Player::MakeAttackMotionOfZkoo()
{
	if (attackTask.IsEnded())
	{
		attackTask.SetEndPos(zkoo.Right()->Position());
		attackTask.ShiftIsEnded();

		//Line������̂��߂ɕK�v�ȍŒ��𒴂�����
		if (CheckLineDef(attackTask.HitLine()))
		{
			line = MakeLongLine(attackTask.HitLine());
			Effect effect = MakeEffect(line.startPos, line.endPos);
			slashEffect.Set(effect.pos, effect.size, effect.angle);
			isAttack = true;
		}
		attackTask.Reset();
	}
	//�l�̏�����
	if (attackTask.IsMotioning() == true)
	{
		attackTask.Reset();
	}
}

void Player::Touch()
{

	//CONSOLE << nowCommand << std::endl;

	auto touch = inputs.touch();
	auto ids = inputs.GetTouchHandleIDs();
	UpdateDeffenceOfTouch();

	for (auto id : ids)
	{
		SetAttackMotionOfTouch(touch, id);
		MakeAttackMotionOfTouch(touch, id);
	}
}

void Player::SetAttackMotionOfTouch(cinder::app::TouchEvent::Touch touch, uint32_t id)
{
	if (inputs.isPressTouch(id, touch))
	{
		attackTask.Update();

		if (slashEffect.Active() == true)
		{
			return;
		}

		//Motion�ɓ����Ă��Ȃ��ꍇ
		if (attackTask.IsMotioning() == false && slashEffect.Active() == false && nowCommand == CommandType::NONE)
		{
			attackTask.ShiftIsMotioning();
			nowCommand = CommandType::ATTACK;
		}

		//Slash�̊J�n���ԂɂȂ�����E��̈ʒu��`��̍ŏ��̈ʒu�ɓ���܂�
		if (attackTask.IsStarted() == true)
		{
			attackTask.SetStartPos(touch.getPos());
			attackTask.ShiftIsStarted();
		}
	}
}

void Player::MakeAttackMotionOfTouch(cinder::app::TouchEvent::Touch touch, uint32_t id)
{
	if (inputs.isPullTouch(id, touch))
	{
		//�I�����Ă����Ȃ�
		if (attackTask.IsEnded() == true)
		{
			attackTask.SetEndPos(touch.getPos());
			attackTask.ShiftIsEnded();

			//Line������̂��߂ɕK�v�ȍŒ��𒴂�����
			if (CheckLineDef(attackTask.HitLine()))
			{
				line = attackTask.HitLine();
				Effect effect = MakeAttackEffect(line.startPos, line.endPos);
				slashEffect.Set(effect.pos, effect.size, effect.angle);
				isAttack = true;
				nowCommand = CommandType::NONE;
			}
			//�l�̏�����
			attackTask.Reset();

		}
		if (nowCommand == CommandType::ATTACK)
		{
			attackTask.Reset();
			nowCommand = CommandType::NONE;
		}
	}
	auto ids = inputs.GetTouchHandleIDs();
		//��O����
		//ex) �}�ɉ�ʊO�ɂ������� �Ȃ�
		if (attackTask.IsMotioning() == true && static_cast<int>(ids.size()) == 0)
		{
			attackTask.Reset();
			nowCommand = CommandType::NONE;
		}

}

void Player::AttackPhase()
{
	Zkoo();
	Touch();
	slashEffect.Update();
}

void Player::UpdateDeffenceOfTouch()
{
	auto touch = inputs.touch();
	auto touch2 = inputs.touch();
	auto ids = inputs.GetTouchHandleIDs();
	for (auto id1 : ids)
	{
		for (auto id2 : ids)
		{

			if (id1 != id2 && inputs.isPressTouch(id1, touch) && inputs.isPressTouch(id2, touch2))
			{
				if (attackTask.IsMotioning() == true)
				{
					attackTask.Reset();
					nowCommand = CommandType::NONE;
				}

				if (deffence.IsActive() == false && nowCommand == CommandType::NONE)
				{
					deffence.ShiftIsActive();
					nowCommand = CommandType::GUARD;
				}

				if (deffence.IsActive() == true)
				{
					deffence.Update(touch.getPos(), touch2.getPos());
				}
			}

			if (deffence.IsActive() && !inputs.isPressTouch(id1, touch) && !inputs.isPressTouch(id2, touch2))
			{
				deffence.ShiftIsActive();
				nowCommand = CommandType::NONE;
			}
		}
	}
}


void Player::Draw()
{
	slashEffect.Draw();
	deffence.Draw();
}

void Player::Update()
{
	if (inputs.isPushKey(Key::KEY_m) && inputs.isPressKey(Key::KEY_LCTRL)) {
		nowMp = maxMp;
	}
    if ( inputs.isPushKey( Key::KEY_h ) && inputs.isPressKey( Key::KEY_LCTRL ) ) {
        nowHp = maxHp;
    }
}
