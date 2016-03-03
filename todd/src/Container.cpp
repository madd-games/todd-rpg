/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014-2016, Madd Games.
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
 * Container.cpp
 * Manages item containers that are saved in save files.
 */

#include "Container.h"
#include "Character.h"
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
		text.draw(x+24, y+16, Text::RIGHT);
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

int Container::drawItemStatInfo(int leftX, string statName, int value, int y)
{
	if (value == 0) return 0;

	Text txtName(statName, 0, 0, 255, 255, fntText);
	txtName.draw(leftX+5, y);

	int red=0, green=0;
	if (value < 0)
	{
		red = 255;
		green = 0;
	}
	else
	{
		red = 0;
		green = 255;
	};

	stringstream ss;
	if (value > 0) ss << "+";
	ss << value;
	Text txtValue(ss.str(), red, green, 0, 255, fntText);
	txtValue.draw(leftX+50, y);

	return 18;
};

void Container::drawInfoPanel(int leftX, Item *item)
{
	if (item != NULL)
	{
		ssElements->draw(leftX+2, 5, item->getElement(), false);
		ssItems->draw(leftX+28, 5, item->id, false);
		Text txtName(item->getName(), 255, 255, 255, 255, fntItemName);
		txtName.draw(leftX+54, 5);
		Text txtDesc(item->getDesc(), 255, 255, 255, 255, fntText, 300);
		txtDesc.draw(leftX+2, 34);

		int drawY = 350;
		CharStats stats;
		memset(&stats, 0, sizeof(CharStats));
		item->getStat(stats);

		drawY += drawItemStatInfo(leftX, "STR", stats.STR, drawY);
		drawY += drawItemStatInfo(leftX, "INT", stats.INT, drawY);
		drawY += drawItemStatInfo(leftX, "DEF", stats.DEF, drawY);
		drawY += drawItemStatInfo(leftX, "MDEF", stats.MDEF, drawY);
	};
};

bool TryTakeAwayItem(int id, int count)
{
	int totalCount = 0;

	int i;
	for (i=0; i<4; i++)
	{
		string member = GetPartyMember(i);
		if (member != "")
		{
			Character *chr = GetChar(member);
			totalCount += chr->getInventory()->count(id);
		};
	};

	if (totalCount < count)
	{
		return false;
	};

	// OK, they've got the item, it's all fine and dandy.
	// now remove it.
	for (i=0; i<4; i++)
	{
		string member = GetPartyMember(i);
		if (member != "")
		{
			Container *inv = GetChar(member)->getInventory();
			int toRemove = inv->count(id);
			if (toRemove > count) toRemove = count;
			count -= toRemove;
			while (toRemove--) inv->decrItem(id);
		};
	};

	return true;
};
