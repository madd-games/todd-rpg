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
 * StatusEffect.h
 * Handling status effects.
 */

#ifndef STATUS_EFFECT_H
#define STATUS_EFFECT_H

#include <inttypes.h>
#include <string>

using namespace std;

class StatusEffect
{
public:
	/**
	 * List of status effects.
	 */
	enum
	{
		POISON = 0,
		BURN = 1,

		REGEN = 16,
		SHIELD = 17,

		COUNT
	};

	virtual ~StatusEffect();

	/**
	 * \brief Returns the element associated with this status effect.
	 */
	virtual int getElement() = 0;

	/**
	 * \brief Returns the name of this status effect.
	 */
	virtual string getName() = 0;

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

	/**
	 * \brief Called when a character with this status effect performs an attack.
	 *
	 * The arguments are references to data internally handled by BattleView::attack().
	 * You can modify the values to affect the outcome. 'damage' already has resistances
	 * etc. calculated.
	 *
	 * Default: do nothing.
	 */
	virtual void onDealDamage(int me, int &target, int &type, int &element, int &damage);

	/**
	 * \brief Called when a character with this status effet receives damage.
	 *
	 * Similarly to onDealDamage(), you can modify the data about this attack. onDealDamage()
	 * was called before this function if the attacker had any status effects.
	 *
	 * Default: do nothing.
	 */
	virtual void onReceiveDamage(int attacker, int &target, int &type, int &element, int &damage);
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

	/**
	 * \brief Test if a status effect is active.
	 * \param effect The effect to test.
	 * \sa StatusEffect
	 */
	bool test(int effect);

	/**
	 * \brief Change the state of a status effect.
	 * \param effect The effect whose state shall be changed.
	 * \param state The state; if true, this status effect is active.
	 * \sa StatusEffect
	 */
	void set(int effect, bool state);

	/**
	 * \brief Clear all status effects, positive and negative.
	 */
	void clear();

	/**
	 * \brief Clear all positive status effects.
	 */
	void clearPositive();

	/**
	 * \brief Apply all status effects to the entity.
	 */
	void onTurn(int entity);
};

StatusEffect *GetStatusEffect(int effect);
void renderStatusEffectSet(int x, int y, StatusEffectSet ses);

#endif
