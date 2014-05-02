/**
 * EnemyGoblin.cpp
 */

#include "EnemyGoblin.h"
#include "SpriteSheet.h"
#include "Element.h"
#include "Skill.h"
#include "BattleView.h"

EnemyGoblin::EnemyGoblin()
{
	spriteSheet = ssGoblin;
	level = 3;
	name = "Goblin";

	hp = maxhp = 20;
	element = Element::EARTH;

	memset(&stats, 0, sizeof(CharStats));
	stats.STR = 0;
	stats.DEF = -1;

	memset(resist, 0, sizeof(int)*Element::NUM_ELEMENTS);
	resist[Element::EARTH] = 10;
	resist[Element::AIR] = -50;
	resist[Element::WATER] = -10;
	resist[Element::FIRE] = -10;
};

Skill *EnemyGoblin::plan()
{
	skillAttack->init(battleView.getRandomAlly());
	return skillAttack;
};
