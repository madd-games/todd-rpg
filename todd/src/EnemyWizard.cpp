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
 * EnemyWizard.cpp
 */

#include "EnemyWizard.h"
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

EnemyWizard::EnemyWizard()
{
	spriteSheet = ssWizard;
	level = 7;
	name = "Wizard";

	hp = maxhp = 120;
	element = Element::EARTH;

	memset(&stats, 0, sizeof(CharStats));
	stats.STR = 5;
	stats.DEF = 8;
	stats.INT = 5;
	stats.MDEF = 8;
	stats.ACC = 20;
	stats.AGI = 50;

	memset(resist, 0, sizeof(int)*Element::NUM_ELEMENTS);
	resist[Element::EARTH] = 80;
	resist[Element::AIR] = -100;
	resist[Element::WATER] = 50;
	resist[Element::FIRE] = 50;
};

Skill *EnemyWizard::plan()
{	
	if (Probably(50))
	{
		skillAttack->init(battleView.getRandomAlly());
		return skillAttack;
	}
	else if (Probably(20))
	{
		skillSweetPotion->init(battleView.getRandomEnemy());
		return skillSweetPotion;
	}
	else
	{
		skillPotion->init(battleView.getRandomEnemy());
		return skillPotion;
	};
};

void EnemyWizard::dropItems(vector<int> &drops)
{
	if (Probably(50))
	{
		drops.push_back(Item::POTION);
	};
	
	if (Probably(20))
	{
		drops.push_back(Item::ANTIDOTE);
	};
	
	if (Probably(1))
	{
		drops.push_back(Item::SWEET_POTION);
	};
};
