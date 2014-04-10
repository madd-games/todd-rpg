/**
 * Item.cpp
 * Describes items.
 */

#include "Item.h"
#include "Todd.h"
#include <map>
#include <sstream>

using namespace std;

map<int, Item*> itemMap;

Item::Item(int id)
{
	RegisterItem(id, this);
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
