/**
 * ItemWoodenShield.h
 */

#include "Item.h"
#include "Element.h"

class ItemWoodenShield : public Item
{
public:
	ItemWoodenShield(int id) : Item(id)
	{
	};

	virtual int getElement()
	{
		return Element::NEUTRAL;
	};

	virtual string getName()
	{
		return "Wooden Shield";
	};

	virtual string getDesc()
	{
		return "A very basic shield made of wood. Increases your defence by 1 point.";
	};

	virtual bool isStackable()
	{
		return false;
	};

	virtual int getType()
	{
		return Item::SHIELD;
	};

	virtual void getStat(CharStats &stats)
	{
		stats.DEF = 1;
	};
};
