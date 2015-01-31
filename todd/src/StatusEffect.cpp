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
 * StatusEffect.cpp
 * Handling status effects.
 */

#include "StatusEffect.h"
#include "BattleView.h"
#include "Element.h"
#include "SpriteSheet.h"

// Include status effect classes here.
#include "SEPoison.h"

StatusEffect::~StatusEffect()
{
};

void StatusEffect::onTurn(int index)
{
	(void)index;
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

SEPoison sePoison;

StatusEffect *GetStatusEffect(int effect)
{
	switch (effect)
	{
	case StatusEffect::POISON:
		return &sePoison;
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
			ssStatus->draw(x+16*i, y, i, false);
		};
	};
};
