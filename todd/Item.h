/**
 * Item.h
 * Describes items.
 */

#ifndef ITEM_H
#define ITEM_H

#include "Element.h"
#include <string>

using namespace std;

/**
 * Base class for all items.
 */
class Item
{
public:
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
};

struct ItemStack
{
	int id;
	int amount;
};

#endif
