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

	virtual int getCost()
	{
		return 400;
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
