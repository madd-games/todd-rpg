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
