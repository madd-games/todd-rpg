/**
 * ItemBasicSword.h
 */

#include "Item.h"
#include "Element.h"

class ItemBasicSword : public Item
{
public:
	ItemBasicSword(int id) : Item(id)
	{
	};

	virtual int getElement()
	{
		return Element::NEUTRAL;
	};

	virtual string getName()
	{
		return "Basic Sword";
	};

	virtual string getDesc()
	{
		return "A very basic sword, with a force of 10.";
	};

	virtual bool isStackable()
	{
		return false;
	};

	virtual int getType()
	{
		return Item::WEAPON;
	};

	virtual void getStat(CharStats &stats)
	{
		stats.STR = 10;
	};
};
