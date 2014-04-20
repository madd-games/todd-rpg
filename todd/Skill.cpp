/**
 * Skill.cpp
 * A class for representing skills used in battles.
 */

#include "Skill.h"
#include "BattleView.h"
#include "Timer.h"

// Include skills here.
#include "SkillAttack.h"

bool Skill::isUsableAgainstDead()
{
	return false;
};

void Skill::init(int target)
{
	(void)target;
};


