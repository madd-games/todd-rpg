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
