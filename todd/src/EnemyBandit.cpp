/**
 * EnemyBandit.cpp
 */

#include "EnemyBandit.h"
#include "SpriteSheet.h"
#include "Element.h"
#include "Skill.h"
#include "BattleView.h"
#include "Mob.h"

EnemyBandit::EnemyBandit()
{
	spriteSheet = GetMobInfo("MOBBANDIT").mobSprite;
	level = 5;
	name = "Bandit";

	hp = maxhp = 500;
	element = Element::DARKNESS;

	memset(&stats, 0, sizeof(CharStats));
	stats.STR = 15;
	stats.DEF = 10;

	memset(resist, 0, sizeof(int)*Element::NUM_ELEMENTS);
	resist[Element::FIRE] = -5;
	resist[Element::LIGHT] = -100;
	resist[Element::DARKNESS] = 200;
};

Skill *EnemyBandit::plan()
{
	skillAttack->init(battleView.getRandomAlly());
	return skillAttack;
};
