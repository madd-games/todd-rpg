/**
 * Container.cpp
 * Manages item containers that are saved in save files.
 */

#include "Container.h"
#include "GameState.h"
#include "SpriteSheet.h"
#include <sstream>
#include "Text.h"

using namespace std;

Container::Container(string name, int size) : name(name), size(size)
{
};

ItemStack Container::get(int index)
{
	ItemStack *items = (ItemStack*) GetGameData(name, size*sizeof(ItemStack));
	return items[index];
};

void Container::set(int index, ItemStack stack)
{
	ItemStack *items = (ItemStack*) GetGameData(name, size*sizeof(ItemStack));
	items[index] = stack;
};

int Container::getSize()
{
	return size;
};

void Container::drawSlot(int x, int y, int index, bool sel)
{
	int si = 0;
	if (sel) si = 1;
	ssSlot->draw(x, y, si, false);
	ItemStack stack = get(index);
	ssItems->draw(x+1, y+1, stack.id, false);

	if (stack.amount > 1)
	{
		stringstream ss;
		ss << stack.amount;
		Text text(ss.str(), 255, 255, 255, 255, fntItemCount);
		text.draw(x+12, y+16);
	};
};

void Container::pushStack(ItemStack stack, int min)
{
	Item *item = GetItem(stack.id);
	if (!item->isStackable())
	{
		int i;
		for (i=min; i<getSize(); i++)
		{
			ItemStack stk = get(i);
			if (stk.id == 0)
			{
				set(i, stack);
				break;
			};
		};
	}
	else
	{
		while (stack.amount != 0)
		{
			int i;
			bool found = false;
			for (i=min; i<getSize(); i++)
			{
				ItemStack stk = get(i);
				if ((stk.id == stack.id) && (stk.amount < 64) && (stack.amount > 0))
				{
					stk.amount++;
					stack.amount--;
					set(i, stk);
					found = true;
				};
			};

			if (!found)
			{
				break;
			};
		};

		if (stack.amount != 0)
		{
			int i;
			for (i=min; i<getSize(); i++)
			{
				ItemStack stk = get(i);
				if (stk.id == 0)
				{
					set(i, stack);
					break;
				};
			};
		};
	};
};

int Container::count(int id)
{
	int result = 0;
	int i;
	for (i=0; i<getSize(); i++)
	{
		ItemStack stack = get(i);
		if (stack.id == id) result += stack.amount;
	};
	return result;
};

void Container::decrItem(int id)
{
	int i;
	for (i=0; i<getSize(); i++)
	{
		ItemStack stack = get(i);
		if (stack.id == id)
		{
			stack.amount--;
			if (stack.amount == 0) stack.id = 0;
			set(i, stack);
			break;
		};
	};
};
