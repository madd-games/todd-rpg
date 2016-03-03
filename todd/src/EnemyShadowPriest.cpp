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
 * EnemyShadowPriest.cpp
 */

#include "EnemyShadowPriest.h"
#include "SpriteSheet.h"
#include "Element.h"
#include "Skill.h"
#include "BattleView.h"
#include <stdlib.h>
#include <time.h>
#include "Item.h"
#include <random>
#include "Mob.h"
#include "Todd.h"

using namespace std;

EnemyShadowPriest::EnemyShadowPriest()
{
	spriteSheet = GetMobInfo("MOBSHPRIEST").mobSprite;
	level = 15;
	name = "Shadow Priest";

	hp = maxhp = 1200;
	element = Element::DARKNESS;

	memset(&stats, 0, sizeof(CharStats));
	stats.STR = 15;
	stats.DEF = 20;
	stats.MDEF = 5;
	stats.INT = 10;

	memset(resist, 0, sizeof(int)*Element::NUM_ELEMENTS);
	resist[Element::EARTH] = 30;
	resist[Element::AIR] = 30;
	resist[Element::WATER] = 30;
	resist[Element::FIRE] = -20;
	resist[Element::DARKNESS] = 200;
	resist[Element::LIGHT] = -100;
};

int EnemyShadowPriest::getAllyWithNoPoison()
{
	int i;
	for (i=0; i<4; i++)
	{
		if (battleView.canMove(i))
		{
			if (!battleView.hasStatusEffect(i, StatusEffect::POISON))
			{
				return i;
			};
		};
	};

	return -1;
};

Skill *EnemyShadowPriest::plan()
{
	if (hp < 650)
	{
		if (Probably(50))
		{
			skillApocalypse->init(0);
			return skillApocalypse;
		}
		else
		{
			skillFireSlash->init(battleView.getRandomAlly());
			return skillFireSlash;
		};
	}
	else
	{
		int allyWithNoPoison = getAllyWithNoPoison();
		if (Probably(70) || (allyWithNoPoison == -1))
		{
			skillFireSlash->init(battleView.getRandomAlly());
			return skillFireSlash;
		}
		else
		{
			skillPoison->init(allyWithNoPoison);
			return skillFireSlash;
		};
	};
};

void EnemyShadowPriest::dropItems(vector<int> &drops)
{
	drops.push_back(Item::STEEL_DAGGER);
	if (Probably(10))
	{
		drops.push_back(Item::CHAIN_ARMOR);
	};
};
