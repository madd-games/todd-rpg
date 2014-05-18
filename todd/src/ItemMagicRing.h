/**
 * ItemMagicRing.h
 */

#include "Item.h"
#include "Element.h"

class ItemMagicRing : public Item
{
public:
	ItemMagicRing(int id) : Item(id)
	{
	};

	virtual int getElement()
	{
		return Element::DARKNESS;
	};

	virtual string getName()
	{
		return "Magic Ring";
	};

	virtual string getDesc()
	{
		return "This magic ring enhances the wearer's magical performance by 2%, but decreases physical performance by 1%.";
	};

	virtual bool isStackable()
	{
		return false;
	};

	virtual int getType()
	{
		return Item::ACCESSORY;
	};

	virtual void getStat(CharStats &stats)
	{
		stats.INT = 2;
		stats.MDEF = 2;
		stats.STR = -1;
		stats.DEF = -1;
	};
};
