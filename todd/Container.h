/**
 * Container.h
 * Manages item containers that are saved in save files.
 */

#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include "Item.h"

using namespace std;

class Container
{
private:
	/**
	 * Names the "game state segment" that identifies this container.
	 */
	string name;

	/**
	 * Size of this container (width * height)
	 */
	int size;

public:
	Container(string name, int size);

	/**
	 * Returns the item stack at the specified slot.
	 */
	ItemStack get(int index);

	/**
	 * Set the item stack at the specified slot.
	 */
	void set(int index, ItemStack stack);

	/**
	 * Return the size of the container.
	 */
	int getSize();
};

#endif
