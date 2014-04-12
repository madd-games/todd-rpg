/**
 * ItemPotion.cpp
 */

#include "Item.h"
#include "Element.h"

class ItemPotion : public Item
{
public:
	ItemPotion(int id) : Item(id)
	{
	};

	virtual int getElement()
	{
		return Element::NEUTRAL;
	};

	virtual string getName()
	{
		return "Potion";
	};

	virtual string getDesc()
	{
		return "A simple remedy, which restores 100HP.";
	};

	virtual bool isStackable()
	{
		return true;
	};
};
