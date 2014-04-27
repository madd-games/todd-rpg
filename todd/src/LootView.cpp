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
		else if ((ev->key.keysym.sym == SDLK_DOWN) || (ev->key.keysym.sym == SDLK_x))
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
				currentView = &sceneView;
			};
		};
	};
};

void LootView::render()
{
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
