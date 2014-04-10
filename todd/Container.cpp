/**
 * Container.cpp
 * Manages item containers that are saved in save files.
 */

#include "Container.h"
#include "GameState.h"

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
