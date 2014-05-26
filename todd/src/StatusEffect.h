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
 * StatusEffect.h
 * Handling status effects.
 */

#ifndef STATUS_EFFECT_H
#define STATUS_EFFECT_H

#include <inttypes.h>

class StatusEffect
{
public:
	/**
	 * List of status effects.
	 */
	enum
	{
		POISON = 0,
		REGEN = 1,
	};

	virtual ~StatusEffect();

	/**
	 * \brief Returns true if this status effect is positive.
	 */
	virtual bool isPositive() = 0;

	/**
	 * \brief Called when a character's turn is scheduled and that character
	 * has this status effect.
	 *
	 * Default: do nothing.
	 */
	virtual void onTurn(int entity);
};

/**
 * This class is part of the CharState structure, and should be 64 bits.
 * It basically helps manage the set of status effects of a character.
 */
class StatusEffectSet
{
private:
	uint64_t bitset;

public:
	StatusEffectSet();
	StatusEffectSet(const StatusEffectSet &ses);
	StatusEffectSet& operator=(StatusEffectSet ses);

	bool test(int effect);
	void set(int effect, bool state);
};

StatusEffect *GetStatusEffect(int effect);

#endif