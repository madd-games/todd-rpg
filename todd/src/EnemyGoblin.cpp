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
 * EnemyGoblin.cpp
 */

#include "EnemyGoblin.h"
#include "SpriteSheet.h"
#include "Element.h"
#include "Skill.h"
#include "BattleView.h"
#include <stdlib.h>
#include <time.h>
#include "Item.h"
#include <random>

using namespace std;

EnemyGoblin::EnemyGoblin()
{
	spriteSheet = ssGoblin;
	level = 3;
	name = "Goblin";

	hp = maxhp = 30;
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

int getProb()
{
	random_device rd;
	default_random_engine e1(rd());
	uniform_int_distribution<int> mknum(0, 99);
	return mknum(e1);
};

void EnemyGoblin::dropItems(vector<int> &drops)
{
	srand(time(NULL));

	if (getProb() < 40)
	{
		drops.push_back(Item::MANA_FRUIT);
	};

	if (getProb() < 30)
	{
		drops.push_back(Item::POTION);
	};

	if (getProb() < 20)
	{
		drops.push_back(Item::GOBLIN_DUST);
	};
};
