/**
 * Skill.h
 * A class for representing skills used in battles.
 */

#ifndef SKILL_H
#define SKILL_H

#include "Element.h"

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
};

extern Skill *skillAttack;

#endif
