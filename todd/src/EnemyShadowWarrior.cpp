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
 * EnemyShadowWarrior.cpp
 */

#include "EnemyShadowWarrior.h"
#include "SpriteSheet.h"
#include "Element.h"
#include "Skill.h"
#include "BattleView.h"
#include <stdlib.h>
#include <time.h>
#include "Item.h"
#include <random>
#include "Todd.h"

using namespace std;

EnemyShadowWarrior::EnemyShadowWarrior()
{
	poisons = RandomUniform(1, 5);
	poisonsUsed = 0;

	spriteSheet = ssShadowWarrior;
	level = 3;
	name = "Shadow Warrior";

	hp = maxhp = 350;
	element = Element::DARKNESS;

	memset(&stats, 0, sizeof(CharStats));
	stats.STR = 10;
	stats.DEF = 10;
	stats.MDEF = 20;
	stats.ACC = 80;
	stats.AGI = 5;

	memset(resist, 0, sizeof(int)*Element::NUM_ELEMENTS);
	resist[Element::EARTH] = 30;
	resist[Element::AIR] = 30;
	resist[Element::WATER] = 30;
	resist[Element::FIRE] = 20;
	resist[Element::DARKNESS] = 75;
	resist[Element::LIGHT] = -100;
};

Skill *EnemyShadowWarrior::plan()
{
	int chanceOfPoison = 70 - (10 * poisonsUsed);
	if (Probably(chanceOfPoison) && (poisons > 0))
	{
		poisons--;
		poisonsUsed++;
		skillPoison->init(battleView.getRandomAlly());
		return skillPoison;
	}
	else
	{
		skillAttack->init(battleView.getRandomAlly());
		return skillAttack;
	};
};

void EnemyShadowWarrior::dropItems(vector<int> &drops)
{
	if (poisons > 2);
	{
		if (Probably(30))
		{
			drops.push_back(Item::BOTTLE_OF_POISON);
		};
	};

	if (Probably(20))
	{
		drops.push_back(Item::ANTIDOTE);
	};
};
