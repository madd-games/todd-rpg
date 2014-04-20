/**
 * EnemyFeminist.cpp
 * Feminist.
 */

#include "EnemyFeminist.h"
#include "SpriteSheet.h"
#include "Mob.h"
#include "BattleView.h"

EnemyFeminist::EnemyFeminist()
{
	spriteSheet = GetMobInfo("MOBFEMINIST").mobSprite;
	level = 9;
	name = "Feminist";

	hp = maxhp = 350;
	mp = maxmp = 120;
	xp = 400;
	maxxp = 1000;

	element = Element::FIRE;

	memset(&stats, 0, sizeof(CharStats));
	stats.STR = 5;
	stats.DEF = 2;
};

Skill *EnemyFeminist::plan()
{
	skillAttack->init(battleView.getRandomAlly());
	return skillAttack;
};
