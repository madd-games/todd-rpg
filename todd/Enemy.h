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

	/**
	 * \brief Called when it's this enemy's turn - must return the skill that's being used,
	 * and must also initialise it.
	 */
	virtual Skill *plan() = 0;
};

#endif
