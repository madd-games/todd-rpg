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
		else if (ev->key.keysym.sym == SDLK_x)
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
			txtCount.draw(x+15+(48*i), y+298);
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
