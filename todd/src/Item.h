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
 * Item.h
 * Describes items.
 */

#ifndef ITEM_H
#define ITEM_H

#include "Element.h"
#include <string>
#include "CharStats.h"

using namespace std;

class Character;

/**
 * Base class for all items.
 */
class Item
{
public:
	/**
	 * \brief Item IDs.
	 */
	enum
	{
		NONE = 0,
		POTION = 1,
		BASIC_SWORD = 2,
		WOODEN_SHIELD = 3,
		MAGIC_RING = 4,
		CHAIN_ARMOR = 5,
		MANA_FRUIT = 6,
		GOBLIN_DUST = 7,
		SPIRIT_KEY = 8,
		SILVER_DAGGER = 9,
		BOTTLE_OF_POISON = 10,
		ANTIDOTE = 11,
		HOLY_SWORD = 12,
		STEEL_DAGGER = 13,
		SHADOW_ARMOR = 14,
		SHADOW_SHIELD = 15,
		SWEET_POTION = 16,
	};
	/**
	 * \brief Weapon types
	 */
	enum
	{
		// NONE already defined as 0 in this case so is not needed.
		SWORD = 1,
		DAGGER = 2,

		NUM_WEAPON_TYPES = 32
	};

	/**
	 * \brief Item types.
	 */
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
	 * Return the cost of this item.
	 */
	virtual int getCost() = 0;

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
	 * Returns what weapon type the weapon belongs to.
	 */
	virtual int getWeaponType();

	/**
	 * Returns the type of item (default = Item::PLOT).
	 */
	virtual int getType();

	/**
	 * Returns the amount of damage dealt by this item, if it is expendable.
	 * NOTE: Damage (or healing) of items is unaffected by defence etc.
	 * This function is only used when the item is expended in the inventory screen, not during
	 * battle.
	 */
	virtual int getDamage();

	/**
	 * Fills the CharStats structure to which a reference is passed as the argument,
	 * and is initialsied to zero, to reflect the effect on a character's statistics
	 * when this item is equipped.
	 */
	virtual void getStat(CharStats &stats);

	/**
	 * Return the amount of mana that this item restores when expended through the inventory
	 * screen.
	 */
	virtual int getManaRestore();

	/**
	 * Called when the item is being expended to a character. Return true if one instance of this item
	 * is to be removed (i.e. something actually happened), false otherwise.
	 */
	virtual bool expend(Character *chr);
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
