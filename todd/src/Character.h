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
 * Character.h
 * Describes a character that can take part in battles or join the player's party,
 * and their state is maintained.
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Element.h"
#include <string>
#include "SpriteSheet.h"
#include "Container.h"
#include "CharStats.h"
#include "StatusEffect.h"

using namespace std;

struct CharState
{
	int hp, maxhp;
	int mp, maxmp;
	int xp, maxxp;
	int ap, maxap;		/* used for passive skills */
	int level;
	CharStats stats;
	StatusEffectSet ses;
};

struct CharInfo
{
	string mob;

	/**
	 * Specifies the resistances of this character to all elements;
	 * This is given in %, and may be negative.
	 */
	int resist[Element::NUM_ELEMENTS];

	/**
	 * Stats gained per-level.
	 * When this character levels up, those values will be added to their current stats.
	 */
	CharStats levelUpStats;

	CharStats stats;
};

class Character
{
private:
	string name;
	CharInfo *charInfo;
	Container inv;

public:
	Character(string name, CharInfo *info, string invName);
	int getHP();
	int getMaxHP();
	void setHP(int hp, int maxhp = 0);

	int getMP();
	int getMaxMP();
	void setMP(int mp, int maxmp = 0);

	int getXP();
	int getMaxXP();
	void setXP(int xp, int maxxp = 0);
	void train(int xp);

	int getLevel();
	void setLevel(int level);

	int getElement();

	CharStats *getStats();
	int *getResistances();
	StatusEffectSet &getStatusEffectSet();

	/**
	 * \brief Deals damage without checking resistances etc.
	 */
	void dealDirectDamage(int dmg);

	/**
	 * \brief Return the SpriteSheet with this character's sprite.
	 */
	SpriteSheet *getSpriteSheet();

	string getName();

	/**
	 * \brief Returns a container with this character's items.
	 *
	 * Index 0 = the weapon,
	 * Index 1 = the shield,
	 * Index 2 = the helmet,
	 * Index 3 = the armor
	 * Index 4-6 (inclusive) = the accessories,
	 * Index 7-10 = reserved
	 * Index 11+ = a 9x4 area to store items.
	 */
	Container *getInventory();
};

Character *GetChar(string name);
string GetPartyMember(int index);
void SetPartyMember(int index, string name);

#endif
