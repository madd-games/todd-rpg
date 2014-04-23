/**
 * Item.h
 * Describes items.
 */

#ifndef ITEM_H
#define ITEM_H

#include "Element.h"
#include <string>
#include "CharStats.h"

using namespace std;

/**
 * Base class for all items.
 */
class Item
{
public:
	enum
	{
		NONE = 0,
		POTION = 1,
		BASIC_SWORD = 2,
		WOODEN_SHIELD = 3,
	};

	enum
	{
		PLOT = 0,
		EXPENDABLE = 1,
		WEAPON = 2,
		SHIELD = 3,
		HELMET = 4,
		ARMOR = 5,
		ACCESSORY = 6,
	};

	int id;
	Item(int id);

	/**
	 * Return the element associated with the item.
	 */
	virtual int getElement() = 0;

	/**
	 * Return the name of the item.
	 */
	virtual string getName() = 0;

	/**
	 * Return a description of the item.
	 */
	virtual string getDesc() = 0;

	/**
	 * Return true if this item is stackable.
	 */
	virtual bool isStackable() = 0;

	/**
	 * Returns the type of item (default = Item::PLOT).
	 */
	virtual int getType();

	/**
	 * Returns the amount of damage dealt by this item, if it is expendable.
	 * NOTE: Damage (or healing) of items is unaffected by defence etc.
	 */
	virtual int getDamage();

	/**
	 * Fills the CharStats structure to which a reference is passed as the argument,
	 * and is initialsied to zero, to reflect the effect on a character's statistics
	 * when this item is equipped.
	 */
	virtual void getStat(CharStats &stats);
};

struct ItemStack
{
	int id;
	int amount;
};

void InitItems();
void RegisterItem(int id, Item *item);
Item *GetItem(int id);

#endif
