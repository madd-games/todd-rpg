/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014, Madd Games.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer. 
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===============================================================================================
*/

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
	 * \brief Returns the item stack at the specified slot.
	 */
	ItemStack get(int index);

	/**
	 * \brief Set the item stack at the specified slot.
	 * \param index Which slot to update.
	 * \param stack The ItemStack to put there.
	 */
	void set(int index, ItemStack stack);

	/**
	 * \brief Return the size of the container.
	 */
	int getSize();

	/**
	 * \brief Draw a container slot.
	 * \param x The X coordinate to draw at.
	 * \param y The Y coordinate to draw at.
	 * \param index The slot to draw.
	 * \param sel If true, the slot is drawn as selected (mouse hovering).
	 */
	void drawSlot(int x, int y, int index, bool sel = false);

	/**
	 * \brief Pushes an itemstack into the best-fitting places in this container.
	 * \param stack The ItemStack that is to be distributed.
	 * \param min The lowest slot index that this function can drop into.
	 */
	void pushStack(ItemStack stack, int min = 10);

	/**
	 * \brief Count the number of items with the given ID in this container.
	 */
	int count(int id);

	/**
	 * \brief Reduces the number of items with the given ID by 1.
	 */
	void decrItem(int id);

	/**
	 * \brief Draw the stats part of an item's description.
	 *
	 * Returns the height of the drawn text.
	 */
	static int drawItemStatInfo(int leftX, string statName, int value, int y);

	/**
	 * \brief Draw the item info panel.
	 *
	 * The panel is drawn on the left side of the screen. The background is not drawn.
	 * \param leftX X coordinate of the left side of the panel.
	 * \param item The item to draw information for.
	 */
	static void drawInfoPanel(int leftX, Item *item);
};

#endif
