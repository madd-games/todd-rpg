/**
 * Character.h
 * Describes a character that can take part in battles or join the player's party,
 * and their state is maintained.
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Element.h"

struct CharStats
{
	int STR;
	int INT;
	int DEF;
	int MDEF;
};

struct CharState
{
	int hp, maxhp;
	int mp, maxmp;
	int xp, maxxp;
	int ap, maxap;		/* used for passive skills */
	int level;
	CharStats stats;
};

struct CharInfo
{
	/**
	 * Specifies the resistances of this character to all elements;
	 * This is given in %, and may be negative.
	 */
	int resist[NUM_ELEMENTS];

	/**
	 * Stats gained per-level.
	 * When this character levels up, those values will be added to their current stats.
	 */
	CharStats levelUpStats;
};

class Character
{
private:
	string name;
	CharInfo *charInfo;

public:
	Character(string name, CharInfo *info);
	
