/**
 * EnemyBandit.cpp
 */

#include "EnemyBandit.h"
#include "SpriteSheet.h"
#include "Element.h"
#include "Skill.h"
#include "BattleView.h"
#include "Mob.h"
#include "Item.h"
#include <stdlib.h>
#include <time.h>

EnemyBandit::EnemyBandit() : numPotions(5), numFireSlash(3)
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
	if ((hp < 200) && (numPotions > 0))
	{
		numPotions--;
		skillPotion->init(4);
		return skillPotion;
	}
	else
	{
		srand(time(NULL));
		if (((rand() % 100) < 45) && (numFireSlash > 0))
		{
			skillFlame->init(battleView.getRandomAlly());
			return skillFlame;
		}
		else
		{
			skillAttack->init(battleView.getRandomAlly());
			return skillAttack;
		};
	};
};

void EnemyBandit::dropItems(vector<int> &drops)
{
	srand(time(NULL));

	drops.push_back(Item::MAGIC_RING);
	if ((rand() % 100) < 50)
	{
		drops.push_back(Item::POTION);
	};
};
