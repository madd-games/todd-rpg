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
 * Enemy.h
 * An abstract class for defining enemies in battles.
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "SpriteSheet.h"
#include <string>
#include "Element.h"
#include "Skill.h"
#include "CharStats.h"
#include <vector>

using namespace std;

class Enemy
{
public:
	SpriteSheet *spriteSheet;
	int level;
	string name;

	int hp, maxhp;
	int mp, maxmp;
	int xp, maxxp;

	int element;
	CharStats stats;
	int resist[Element::NUM_ELEMENTS];

	virtual ~Enemy();

	/**
	 * \brief Called when it's this enemy's turn - must return the skill that's being used,
	 * and must also initialise it.
	 */
	virtual Skill *plan() = 0;

	/**
	 * \brief Called at the end of a successful battle.
	 *
	 * The argument is a reference to a vector of ints, that shall be filled with all the item IDs
	 * that this enemy drops. The vector may already contain elements! By default, this function does
	 * nothing.
	 */
	virtual void dropItems(vector<int> &drops);
};

#endif
