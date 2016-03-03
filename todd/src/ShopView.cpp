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
 * ShopView.cpp
 * A view that allows a player to trade with mobs.
 */

#include "ShopView.h"
#include "Text.h"
#include "Character.h"
#include "Timer.h"
#include "Todd.h"
#include "SceneView.h"
#include "Options.h"
#include "PlotState.h"

#define SHOP_CONT_X		700
#define	SHOP_CONT_Y		150

#define	CHAR_CONT_X		310
#define	CHAR_CONT_Y		SHOP_CONT_Y

#define	CONT_WIDTH		26*9
#define	CONT_HEIGHT		26*4

ShopView shopView;

ShopView::ShopView() : shopCont(NULL)
{
};

void ShopView::init(string contName, string shopName)
{
	if (shopCont != NULL) delete shopCont;
	this->shopName = shopName;
	shopCont = new Container(contName, 9*8);
	charSel = 0;
	handleStack.id = 0;
	handleStack.amount = 0;
};

void ShopView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		int key = ev->key.keysym.sym;
		if (key == SDLK_LEFT)
		{
			if (charSel != 0) charSel--;
		}
		else if (key == SDLK_RIGHT)
		{
			charSel++;
			if (GetPartyMember(charSel) == "") charSel--;
		}
		else if (key == options.cancelKey)
		{
			if (handleStack.id == 0)
			{
				currentView = &sceneView;
			};
		};
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN)
	{
		if (ev->button.button == SDL_BUTTON_LEFT)
		{
			int mx, my;
			SDL_GetMouseState(&mx, &my);

			if ((mx > CHAR_CONT_X) && (mx < CHAR_CONT_X+CONT_WIDTH) && (my > CHAR_CONT_Y) && (my < CHAR_CONT_Y+CONT_HEIGHT))
			{
				int x = (mx - CHAR_CONT_X) / 26;
				int y = (my - CHAR_CONT_Y) / 26;
				int index = y * 9 + x + 10;

				string name = GetPartyMember(charSel);
				Character *chr = GetChar(name);
				ItemStack stack = chr->getInventory()->get(index);
				if ((stack.id == handleStack.id) && (stack.id != 0))
				{
					Item *itm = GetItem(stack.id);
					if (itm->isStackable())
					{
						if (stack.amount < 64)
						{
							stack.amount++;
							handleStack.amount--;
							if (handleStack.amount == 0) handleStack.id = 0;
							chr->getInventory()->set(index, stack);
						};
					};
				}
				else
				{
					Swap<ItemStack>(stack, handleStack);
					chr->getInventory()->set(index, stack);
				};
			};

			if ((mx > SHOP_CONT_X) && (mx < SHOP_CONT_X+CONT_WIDTH) && (my > SHOP_CONT_Y) && (my < SHOP_CONT_Y+CONT_HEIGHT*2))
			{
				int x = (mx - SHOP_CONT_X) / 26;
				int y = (my - SHOP_CONT_Y) / 26;
				int index = y * 9 + x;

				ItemStack stack = shopCont->get(index);
				if (stack.id == 0)
				{
					if (handleStack.id != 0)
					{
						Item *item = GetItem(handleStack.id);
						GetPlotState()->gold += item->getCost()*handleStack.amount;
						Swap<ItemStack>(stack, handleStack);
					};
				}
				else
				{
					Item *item = GetItem(stack.id);
					if (GetPlotState()->gold >= item->getCost())
					{
						if (handleStack.id == 0)
						{
							handleStack.id = stack.id;
						};

						if (handleStack.id == stack.id)
						{
							if (handleStack.amount != 64)
							{
								handleStack.amount++;
								stack.amount--;
								if (stack.amount == 0) stack.id = 0;
								GetPlotState()->gold -= item->getCost();
							};
						};
					};
				};

				shopCont->set(index, stack);
			};

			if (handleStack.id != 0)
			{
				showCursor = false;
			}
			else
			{
				showCursor = true;
			};

		}
		else if (ev->button.button == SDL_BUTTON_RIGHT)
		{
			int mx, my;
			SDL_GetMouseState(&mx, &my);

			if ((mx > CHAR_CONT_X) && (mx < CHAR_CONT_X+CONT_WIDTH) && (my > CHAR_CONT_Y) && (my < CHAR_CONT_Y+CONT_HEIGHT))
			{
				int x = (mx - CHAR_CONT_X) / 26;
				int y = (my - CHAR_CONT_Y) / 26;
				int index = y * 9 + x + 10;

				string name = GetPartyMember(charSel);
				Character *chr = GetChar(name);
				ItemStack stack = chr->getInventory()->get(index);
				if ((stack.id == handleStack.id) && (stack.id != 0))
				{
					Item *item = GetItem(stack.id);
					if (item->isStackable())
					{
						if (handleStack.amount < 64)
						{
							handleStack.amount++;
							stack.amount--;
						};
					};
				}
				else if ((handleStack.id == 0) && (stack.id != 0))
				{
					handleStack.id = stack.id;
					handleStack.amount = 1;
					stack.amount--;
				}
				else if ((handleStack.id != 0) && (stack.id == 0))
				{
					stack.id = handleStack.id;
					stack.amount = 1;
					handleStack.amount--;
				};

				if (stack.amount == 0) stack.id = 0;
				if (handleStack.amount == 0) handleStack.id = 0;
				chr->getInventory()->set(index, stack);
			};

			if ((mx > SHOP_CONT_X) && (mx < SHOP_CONT_X+CONT_WIDTH) && (my > SHOP_CONT_Y) && (my < SHOP_CONT_Y+CONT_HEIGHT*2))
			{
				int x = (mx - SHOP_CONT_X) / 26;
				int y = (my - SHOP_CONT_Y) / 26;
				int index = y * 9 + x;

				ItemStack stack = shopCont->get(index);
				if (handleStack.id != 0)
				{
					if (stack.id == 0)
					{
						stack.id = handleStack.id;
					};

					if (stack.amount != 64)
					{
						handleStack.amount--;
						stack.amount++;
						GetPlotState()->gold += GetItem(stack.id)->getCost();
						if (handleStack.amount == 0) handleStack.id = 0;
					};
				};
				shopCont->set(index, stack);
			};

			if (handleStack.id != 0)
			{
				showCursor = false;
			}
			else
			{
				showCursor = true;
			};
		};
	};
};

void ShopView::render()
{
	Text txtCaption(shopName, 255, 255, 255, 255);
	txtCaption.draw(SHOP_CONT_X, SHOP_CONT_Y-24);

	Character *chr = GetChar(GetPartyMember(charSel));
	Container *charCont = chr->getInventory();

	int csel = -1, shsel = -1;
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	if ((mx > CHAR_CONT_X) && (mx < CHAR_CONT_X+CONT_WIDTH) && (my > CHAR_CONT_Y) && (my < CHAR_CONT_Y+CONT_HEIGHT))
	{
		int x = (mx - CHAR_CONT_X) / 26;
		int y = (my - CHAR_CONT_Y) / 26;
		csel = y * 9 + x + 10;
	};

	if ((mx > SHOP_CONT_X) && (mx < SHOP_CONT_X+CONT_WIDTH) && (my > SHOP_CONT_Y) && (my < SHOP_CONT_Y+CONT_HEIGHT*2))
	{
		int x = (mx - SHOP_CONT_X) / 26;
		int y = (my - SHOP_CONT_Y) / 26;
		shsel = y * 9 + x;
	};

	int i;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			chr->getSpriteSheet()->draw(CHAR_CONT_X+48*i, CHAR_CONT_Y-48, 0, false);
		};
	};

	int cy = CHAR_CONT_Y - 72;
	if ((Timer::Read() % 1000) < 500) cy += 2;
	ssCursor->draw(CHAR_CONT_X+48*charSel+12, cy, 1, false);

	int x, y;
	for (x=0; x<9; x++)
	{
		for (y=0; y<8; y++)
		{
			int index = y * 9 + x;
			shopCont->drawSlot(SHOP_CONT_X+26*x, SHOP_CONT_Y+26*y, index, index == shsel);
			if (index < 36)
			{
				charCont->drawSlot(CHAR_CONT_X+26*x, CHAR_CONT_Y+26*y, index + 10, index + 10 == csel);
			};
		};
	};

	Item *itemSel = NULL;
	if (csel != -1)
	{
		ItemStack stack = charCont->get(csel);
		if (stack.id != 0)
		{
			itemSel = GetItem(stack.id);
		};
	};

	if (shsel != -1)
	{
		ItemStack stack = shopCont->get(shsel);
		if (stack.id != 0)
		{
			itemSel = GetItem(stack.id);
		};
	};

	ssInfoPanel->draw(0, 0, 0, false);
	Text txtCostLabel("Cost:", 255, 255, 255, 255);
	txtCostLabel.draw(3, 454);
	ssCoin->draw(3+txtCostLabel.getWidth(), 454, 0, false);
	if (itemSel != NULL)
	{
		Container::drawInfoPanel(0, itemSel);
		stringstream ss;
		ss << itemSel->getCost();
		Text txtCost(ss.str(), 255, 255, 255, 255);
		txtCost.draw(3+txtCostLabel.getWidth()+24, 454);
	};

	if (handleStack.id != 0)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		ssItems->draw(x-13, y-13, handleStack.id, false, 24, 26, 24, 26);
		if (handleStack.amount > 1)
		{
			stringstream ss;
			ss << handleStack.amount;
			Text text(ss.str(), 255, 255, 255, 255, fntItemCount);
			text.draw(x+10, y+3, Text::RIGHT);
		};
	};

	ssCoin->draw(CHAR_CONT_X+192, CHAR_CONT_Y-48, 0, false);
	stringstream ss;
	ss << GetPlotState()->gold;
	Text txtGold(ss.str(), 255, 255, 255, 255);
	txtGold.draw(CHAR_CONT_X+216, CHAR_CONT_Y-48);
};
