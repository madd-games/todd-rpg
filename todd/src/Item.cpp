/**
 * Item.cpp
 * Describes items.
 */

#include "Item.h"
#include "Todd.h"
#include <map>
#include <sstream>
#include <iostream>

// Include item classes here.
#include "ItemPotion.h"
#include "ItemBasicSword.h"
#include "ItemWoodenShield.h"
#include "ItemMagicRing.h"

using namespace std;

map<int, Item*> itemMap;

Item::Item(int id) : id(id)
{
	RegisterItem(id, this);
};

int Item::getType()
{
	return Item::PLOT;
};

int Item::getDamage()
{
	return 0;
};

void Item::getStat(CharStats &stats)
{
};

void RegisterItem(int id, Item *item)
{
	if (itemMap.count(id) != 0)
	{
		stringstream ss;
		ss << "Item with ID " << id << " is duplicated.";
		GameAbort(ss.str());
	};

	itemMap[id] = item;
};

Item *GetItem(int id)
{
	return itemMap[id];
};

void InitItems()
{
	itemMap.clear();
	new ItemPotion(Item::POTION);
	new ItemBasicSword(Item::BASIC_SWORD);
	new ItemWoodenShield(Item::WOODEN_SHIELD);
	new ItemMagicRing(Item::MAGIC_RING);
};
