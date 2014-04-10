/**
 * Character.h
 * Describes a character that can take part in battles or join the player's party,
 * and their state is maintained.
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Element.h"
#include <string>

using namespace std;

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
};

class Character
{
private:
	string name;
	CharInfo *charInfo;

public:
	Character(string name, CharInfo *info);
	int getHP();
	int getMaxHP();
	void setHP(int hp, int maxhp = 0);
};

Character *GetChar(string name);
string GetPartyMember(int index);
void SetPartyMember(int index, string name);

#endif
