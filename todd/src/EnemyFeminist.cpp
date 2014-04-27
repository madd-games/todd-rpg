/**
 * EnemyFeminist.cpp
 * Feminist.
 */

#include "EnemyFeminist.h"
#include "SpriteSheet.h"
#include "Mob.h"
#include "BattleView.h"
#include "Item.h"
#include <time.h>

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

	memset(resist, 0, sizeof(int)*Element::NUM_ELEMENTS);
	resist[Element::FIRE] = 80;
	resist[Element::WATER] = -100;
	
};

Skill *EnemyFeminist::plan()
{
	srand(time(NULL));
	if ((rand() % 100) < 50)
	{
		skillAttack->init(battleView.getRandomAlly());
		return skillAttack;
	}
	else
	{
		skillHeal->init(4);
		return skillHeal;
	};
};

void EnemyFeminist::dropItems(vector<int> &drops)
{
	drops.push_back(Item::POTION);
	drops.push_back(Item::POTION);
	drops.push_back(Item::BASIC_SWORD);
};
