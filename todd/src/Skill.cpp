/**
 * Skill.cpp
 * A class for representing skills used in battles.
 */

#include "Skill.h"
#include "BattleView.h"
#include "Timer.h"
#include "Item.h"
#include "Character.h"
#include "Todd.h"

// Include skills here.
#include "SkillAttack.h"
#include "SkillHeal.h"
#include "SkillPotion.h"

bool Skill::isUsableAgainstDead()
{
	return false;
};

void Skill::init(int target)
{
	(void)target;
};

string Skill::getDesc()
{
	return "";
};

int Skill::getManaUse()
{
	return 0;
};
