/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014, Madd Games.
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
#include "ItemChainArmor.h"
#include "ItemManaFruit.h"
#include "ItemGoblinDust.h"

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

int Item::getManaRestore()
{
	return 0;
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
	new ItemChainArmor(Item::CHAIN_ARMOR);
	new ItemManaFruit(Item::MANA_FRUIT);
	new ItemGoblinDust(Item::GOBLIN_DUST);
};
