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
 * Skill.cpp
 * A class for representing skills used in battles.
 */

#include "Skill.h"
#include "BattleView.h"
#include "Timer.h"
#include "Item.h"
#include "Character.h"
#include "Todd.h"
#include "StatusEffect.h"
#include "GameState.h"

// Include skills here.
#include "SkillAttack.h"
#include "SkillHeal.h"
#include "SkillPotion.h"
#include "SkillFireSlash.h"
#include "SkillManaFruit.h"
#include "SkillSplash.h"
#include "SkillPoison.h"
#include "SkillAntidote.h"
#include "SkillApocalypse.h"
#include "SkillHealAll.h"
#include "SkillBurn.h"
#include "SkillSuckBlood.h"
#include "SkillShield.h"

bool Skill::isUsableAgainstDead()
{
	return false;
};

void Skill::onUse()
{
	string var;
	int countToLearn, itemID;
	configLearning(var, countToLearn, itemID);
	(*((int*)GetGameData(var, sizeof(int))))++;	// "C/C++ is a simple language"
};

bool Skill::isUseable(Character *chr)
{
	string var;
	int countToLearn, itemID;
	configLearning(var, countToLearn, itemID);

	if (countToLearn == 0)
	{
		return true;
	};

	int soFar = *((int*)GetGameData(var, sizeof(int)));
	if (soFar >= countToLearn)
	{
		return true;
	};

	int i;
	for (i=0; i<10; i++)
	{
		if (chr->getInventory()->get(i).id == itemID)
		{
			return true;
		};
	};

	return false;
};

void Skill::init(int target)
{
	(void)target;
};

string Skill::getDesc()
{
	return "";
};

int Skill::getManaUse()
{
	return 0;
};

bool Skill::isMultiTarget()
{
	return false;
};

void Skill::configLearning(string &countVar, int &countToLearn, int &itemID)
{
	countVar = "SKLNULL";
	countToLearn = 0;
};
