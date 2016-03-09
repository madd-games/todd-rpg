/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014-2016, Madd Games.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer. 
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===============================================================================================
*/

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
	stats.ACC = 80;
	stats.AGI = 10;

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
			numFireSlash--;
			skillFireSlash->init(battleView.getRandomAlly());
			return skillFireSlash;
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
