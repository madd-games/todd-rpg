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
 * ChestGUI.cpp
 * The chest GUI.
 */

#include "ChestGUI.h"
#include "SpriteSheet.h"
#include "Todd.h"
#include "Item.h"
#include "Text.h"
#include <sstream>
#include "Character.h"
#include "Timer.h"
#include "Options.h"

using namespace std;

ChestGUI::ChestGUI(ChestState *state)
{
	id = state->id;
	amount = state->amount;

	state->id = 0;
	state->amount = 0;

	sel = 0;
};

void ChestGUI::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			sel++;
			if (GetPartyMember(sel) == "") sel--;
		}
		else if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (sel != 0) sel--;
		}
		else if (ev->key.keysym.sym == options.confirmKey)
		{
			Container *cont = GetChar(GetPartyMember(sel))->getInventory();
			ItemStack stk;
			stk.id = id;
			stk.amount = 1;
			amount--;
			cont->pushStack(stk);
		};
	};
};

void ChestGUI::render()
{
	int x = (SCREEN_WIDTH*48/2) - 150;
	int y = (SCREEN_HEIGHT*48/2) - 158;

	ssChestGUI->draw(x, y, 0, false);

	Item *item = GetItem(id);
	ssElements->draw(x+2, y+2, item->getElement(), false);
	ssItems->draw(x+28, y+2, id, false);

	Text txtCaption(item->getName(), 255, 255, 255, 255, fntItemName);
	txtCaption.draw(x+54, y);

	stringstream ss;
	ss << "x" << amount;
	Text txtAmount(ss.str(), 255, 255, 255, 255);
	txtAmount.draw(x+270, y+5);

	Text txtDesc(item->getDesc(), 255, 255, 255, 255, fntText, 280);
	txtDesc.draw(x+2, y+30);

	int i;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			chr->getSpriteSheet()->draw(x+2+(48*i), y+250, 0, false);
			stringstream ss2;
			ss2 << chr->getInventory()->count(id);
			Text txtCount(ss2.str(), 255, 255, 255, 255, fntText);
			txtCount.draw(x+26+(48*i), y+298, Text::CENTER);
		};
	};

	int plotY = 226;
	if ((Timer::Read() % 1000) < 500) plotY += 2;
	ssCursor->draw(x+14+(48*sel), y+plotY, 1, false);
};

bool ChestGUI::isLiving()
{
	return amount != 0;
};
