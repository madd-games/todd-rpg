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
 * LootView.cpp
 * The loot view, where the player can give loot to his party after a battle.
 */

#include "LootView.h"
#include "Timer.h"
#include "SpriteSheet.h"
#include "Item.h"
#include "Text.h"
#include "Character.h"
#include <sstream>
#include "SceneView.h"
#include "Todd.h"
#include "Options.h"

using namespace std;

LootView lootView;

void LootView::init(vector<int> &lootVector)
{
	this->lootVector = &lootVector;
	lootIndex = 0;
	chrSel = 0;
};

void LootView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (chrSel != 0) chrSel--;
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			chrSel++;
			if (GetPartyMember(chrSel) == "") chrSel--;
		}
		else if ((ev->key.keysym.sym == SDLK_DOWN) || (ev->key.keysym.sym == options.confirmKey))
		{
			Character *chr = GetChar(GetPartyMember(chrSel));
			Container *cont = chr->getInventory();

			ItemStack stack;
			stack.id = (*lootVector)[lootIndex];
			stack.amount = 1;
			cont->pushStack(stack);

			lootIndex++;

			if (lootIndex == (int)lootVector->size())
			{
				sceneView.keyLeft = sceneView.keyRight = sceneView.keyUp = sceneView.keyDown = false;
				currentView = &sceneView;
			};
		};
	};
};

void LootView::render()
{
	if (lootVector->size() == 0)
	{
		sceneView.keyLeft = sceneView.keyRight = sceneView.keyUp = sceneView.keyDown = false;
		currentView = &sceneView;
		return;
	};

	int plotX = 768;
	int i;

	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			chr->getSpriteSheet()->draw(plotX, 24, 0, false);

			if (i == chrSel)
			{
				int y = 2;
				if ((Timer::Read() % 1000) < 500) y += 2;
				ssCursor->draw(plotX+12, y, 1, false);
			};
		};
		plotX += 48;
	};

	int plotY = 74;
	for (i=0; i<(int)lootVector->size(); i++)
	{
		if (i >= lootIndex)
		{
			if (i == lootIndex)
			{
				int x = 2;
				if ((Timer::Read() % 1000) < 500) x += 2;
				ssCursor->draw(x, plotY, 0, false);
			};

			int id = (*lootVector)[i];
			Item *item = GetItem(id);
			ssItems->draw(24, plotY, id, false);
			Text text(item->getName(), 255, 255, 255, 255, fntItemName);
			text.draw(48, plotY-2);

			int j;
			plotX = 792;
			for (j=0; j<4; j++)
			{
				string name = GetPartyMember(j);
				if (name != "")
				{
					stringstream ss;
					ss << GetChar(name)->getInventory()->count(id);
					Text text(ss.str(), 255, 255, 255, 255);
					text.draw(plotX, plotY, Text::CENTER);
				};

				plotX += 48;
			};
		};

		plotY += 24;
	};
};
