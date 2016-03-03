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
 * StatusEffect.cpp
 * Handling status effects.
 */

#include "StatusEffect.h"
#include "BattleView.h"
#include "Element.h"
#include "SpriteSheet.h"

// Include status effect classes here.
#include "SEPoison.h"
#include "SEBurn.h"
#include "SERegen.h"
#include "SEShield.h"

StatusEffect::~StatusEffect()
{
};

void StatusEffect::onTurn(int index)
{
	(void)index;
};

void StatusEffect::onDealDamage(int me, int &target, int &type, int &element, int &damage)
{
};

void StatusEffect::onReceiveDamage(int attacker, int &target, int &type, int &element, int &damage)
{
};

StatusEffectSet::StatusEffectSet() : bitset(0)
{
};

StatusEffectSet::StatusEffectSet(const StatusEffectSet &ses) : bitset(ses.bitset)
{
};

StatusEffectSet& StatusEffectSet::operator=(StatusEffectSet ses)
{
	bitset = ses.bitset;
	return *this;
};

bool StatusEffectSet::test(int effect)
{
	return !!(bitset & (1 << effect));
};

void StatusEffectSet::set(int effect, bool state)
{
	int mask = (1 << effect);
	if (state)
	{
		bitset |= mask;
	}
	else
	{
		bitset &= ~mask;
	};
};

void StatusEffectSet::clear()
{
	bitset = 0;
};

void StatusEffectSet::clearPositive()
{
	int i;
	for (i=0; i<StatusEffect::COUNT; i++)
	{
		StatusEffect *se = GetStatusEffect(i);
		if (se != NULL)
		{
			if (se->isPositive())
			{
				set(i, false);
			};
		};
	};
};

void StatusEffectSet::onTurn(int entity)
{
	int i;
	for (i=0; i<StatusEffect::COUNT; i++)
	{
		if (test(i))
		{
			StatusEffect *se = GetStatusEffect(i);
			se->onTurn(entity);
		};
	};
};

SEPoison	sePoison;
SEBurn		seBurn;
SERegen		seRegen;
SEShield	seShield;

StatusEffect *GetStatusEffect(int effect)
{
	switch (effect)
	{
	case StatusEffect::POISON:
		return &sePoison;
	case StatusEffect::BURN:
		return &seBurn;
	case StatusEffect::REGEN:
		return &seRegen;
	case StatusEffect::SHIELD:
		return &seShield;
	default:
		return NULL;
	};
};

void renderStatusEffectSet(int x, int y, StatusEffectSet ses)
{
	int i;
	for (i=0; i<StatusEffect::COUNT; i++)
	{
		if (ses.test(i))
		{
			int gridY = i / 16;
			int gridX = i % 16;
			ssStatus->draw(x+16*gridX, y+16*gridY, i, false);
		};
	};
};
