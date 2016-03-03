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
 * Skill.h
 * A class for representing skills used in battles.
 */

#ifndef SKILL_H
#define SKILL_H

#include "Element.h"
#include <string>
#include <math.h>

using namespace std;

class Character;

class Skill
{
private:
	bool isUseable(Character *chr);
	void onUse();

public:	
	/**
 	 * \brief Returns true if this skill can be used against dead people.
	 *
	 * Default = false
	 */
	virtual bool isUsableAgainstDead();

	/**
	 * \brief Called when this skill is activated.
	 */
	virtual void init(int target);

	/**
	 * \brief Called repeatedly while this skill is active.
	 */
	virtual void act() = 0;

	/**
	 * \brief Returns false once the skill has completed acting.
	 */
	virtual bool isActive() = 0;

	/**
	 * \brief Returns true if this skill is offensive.
	 */
	virtual bool isOffensive() = 0;

	/**
	 * \brief Return this skill's element.
	 */
	virtual int getElement() = 0;

	/**
	 * \brief Return the name of this skill.
	 */
	virtual string getName() = 0;

	/**
	 * \brief Return the description for this skill (default is an empty string).
	 */
	virtual string getDesc();

	/**
	 * \brief Return the amount of mana used by this skill (default is 0).
	 */
	virtual int getManaUse();

	/**
	 * \brief Returns true if this skill is supposed to be used on a whole party (allies or enemies).
	 *
	 * Default = false
	 */
	virtual bool isMultiTarget();

	/**
	 * \brief Configure the learning of this skill.
	 *
	 * The arguments to this function are references to variables which will be used to decide whether
	 * this skill was learnt. Set "countToLearn" to 0 if this skill is to be learnt immediately (this is
	 * also the default).
	 *
	 * \param countVar Name of the game state variable which indicates how many times this skill was used.
	 *                 Should start with "SKL".
	 * \param countToLearn How many times this skill must be used to be learnt permanently.
	 * \param itemID The item, which when equipped, allows the skill to be used before it is learnt.
	 */
	virtual void configLearning(string &countVar, int &countToLearn, int &itemID);
	
	friend class BattleView;
};

extern Skill *skillAttack;
extern Skill *skillHeal;
extern Skill *skillPotion;
extern Skill *skillFireSlash;
extern Skill *skillManaFruit;
extern Skill *skillSplash;
extern Skill *skillPoison;
extern Skill *skillAntidote;
extern Skill *skillApocalypse;
extern Skill *skillHealAll;
extern Skill *skillBurn;
extern Skill *skillSuckBlood;
extern Skill *skillShield;

#endif
