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
 * ShopView.cpp
 * A view that allows a player to trade with mobs.
 */

#include "ShopView.h"

#define SHOP_CONT_X		700
#define	SHOP_CONT_Y		200

ShopView shopView;

ShopView::ShopView() : shopCont(NULL)
{
};

void ShopView::init(string contName, string shopName)
{
	if (shopCont != NULL) delete shopCont;
	this->shopName = shopName;
	shopCont = new Container(contName, 9*4);
};

void ShopView::handleEvent(SDL_Event *ev)
{
	(void)ev;
};

void ShopView::render()
{
	int x, y;
	for (x=0; x<9; x++)
	{
		for (y=0; y<4; y++)
		{
			shopCont->drawSlot(SHOP_CONT_X+26*x, SHOP_CONT_Y+26*y, y * 9 + x, false);
		};
	};
};
