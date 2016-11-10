#include "AttackFactory.h"
#include "Framework.hpp"

void AttackFactory::Update()
{
	if (NotMotioning())return;

	++frameCount;

	if (StartingAttack())
		isStarted = true;

	else if (EndingAttack())
		isEnded = true;
}