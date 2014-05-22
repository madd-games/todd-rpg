/**
 * Skill.h
 * A class for representing skills used in battles.
 */

#ifndef SKILL_H
#define SKILL_H

#include "Element.h"
#include <string>

using namespace std;

class Skill
{
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
};

extern Skill *skillAttack;
extern Skill *skillHeal;
extern Skill *skillPotion;
extern Skill *skillFlame;

#endif
