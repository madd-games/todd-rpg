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

using namespace std;

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

	int getLevel();
	void setLevel(int level);

	int getElement();

	CharStats *getStats();

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
	 * Index 4-6 = the accessories,
	 * Index 7-10 = reserved
	 * Index 11+ = a 9x4 area to store items.
	 */
	Container *getInventory();
};

Character *GetChar(string name);
string GetPartyMember(int index);
void SetPartyMember(int index, string name);

#endif
