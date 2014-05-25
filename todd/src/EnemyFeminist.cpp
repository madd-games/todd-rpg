/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014, Madd Games.
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
