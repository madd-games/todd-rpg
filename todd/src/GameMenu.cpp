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
 * GameMenu.cpp
 * The game menu.
 */

#include "GameMenu.h"
#include "Timer.h"
#include "Todd.h"
#include "SpriteSheet.h"
#include "Text.h"
#include <sstream>
#include "Character.h"
#include <iostream>
#include "Options.h"
#include "Quest.h"
#include "PlotState.h"
#include "StatusEffect.h"

using namespace std;

#define	ANIM_TIME		1000

const char *gameMenuLabels[] = {
	"Party",
	"Inventory",
	"Quests",
	NULL
};

const char *partyPageNames[] = {
	"STATUS",
	"STATS",
	"RESISTANCES"
};

struct StatLabel
{
	string label;
	int x;
	int y;
	int *aptr;
	int *rptr;
};

GameMenu::GameMenu() : rightX(SCREEN_WIDTH*48), leftX(-660), dying(false), selection(0), xsel(0), qsel(-1), numQuests(0)
{
	timer = Timer::Read();
	handleStack.id = 0;
	handleStack.amount = 0;
};

void GameMenu::handleEvent(SDL_Event *ev)
{
	if (!dying)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if ((ev->key.keysym.sym == options.menuKey) || (ev->key.keysym.sym == options.cancelKey))
			{
				if ((handleStack.id == 0) && (qsel == -1))
				{
					timer = Timer::Read();
					dying = true;
				}
				else if ((qsel != -1) && (ev->key.keysym.sym == options.cancelKey))
				{
					qsel = -1;
				};
			}
			else if (ev->key.keysym.sym == options.confirmKey)
			{
				if ((selection == 2) && (qsel == -1) && (numQuests != 0))
				{
					qsel = 0;
				};
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if ((handleStack.id == 0) && (qsel == -1))
				{
					if (selection != 0)
					{
						selection--;
						xsel = 0;
					};
				}
				else if (qsel != -1)
				{
					if (qsel != 0) qsel--;
				};
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				if ((handleStack.id == 0) && (qsel == -1))
				{
					selection++;
					if (gameMenuLabels[selection] == NULL) selection--;
					else xsel = 0;
				}
				else if (qsel != -1)
				{
					if (qsel != (numQuests-1)) qsel++;
				};
			}
			else if (ev->key.keysym.sym == SDLK_LEFT)
			{
				if (xsel != 0) xsel--;
			}
			else if (ev->key.keysym.sym == SDLK_RIGHT)
			{
				int limit = 0;
				if (selection == 0)			// Party
				{
					limit = 3;
				}
				else if (selection == 1)		// Inventory
				{
					int i;
					for (i=0; i<4; i++)
					{
						if (GetPartyMember(i) == "")
						{
							limit = i;
							break;
						};
					};
				};

				if (xsel < (limit-1)) xsel++;
			};
		}
		else if (ev->type == SDL_MOUSEBUTTONDOWN)
		{
			if (ev->button.button == SDL_BUTTON_LEFT)
			{
				int mx, my;
				int rmx, rmy;
				SDL_GetMouseState(&rmx, &rmy);
				mx = rmx;
				my = rmy;
				mx -= (leftX+309);
				my -= 150;
				mx /= 26;
				my /= 26;

				if ((mx < 9) && (my < 4) && (mx >= 0) && (my >= 0) && (selection == 1))
				{
					string name = GetPartyMember(xsel);
					Character *chr = GetChar(name);
					ItemStack stack = chr->getInventory()->get(10 + my * 9 + mx);
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
								chr->getInventory()->set(10 + my * 9 + mx, stack);
							};
						};
					}
					else
					{
						Swap<ItemStack>(stack, handleStack);
						chr->getInventory()->set(10 + my * 9 + mx, stack);
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
				else if ((rmx > (leftX+309)) && (rmx < (leftX+357)) && (rmy > 256) && (rmy < 304))
				{
					if (handleStack.id != 0)
					{
						Item *item = GetItem(handleStack.id);
						if (item->getType() == Item::EXPENDABLE)
						{
							Character *chr = GetChar(GetPartyMember(xsel));

							if (item->expend(chr))
							{
								handleStack.amount--;
								if (handleStack.amount == 0) handleStack.id = 0;
								if (handleStack.id == 0) showCursor = true;
								else showCursor = false;
							};
						};
					};
				}
				else if ((rmx > (leftX+390)) && (rmx < (leftX+572)) && (rmy > 330) && (rmy < 356))
				{
					int slot = (rmx-(leftX+390))/26;
					if (isItemGoodForSlot(handleStack.id, slot))
					{
						Character *chr = GetChar(GetPartyMember(xsel));
						ItemStack stk = chr->getInventory()->get(slot);
						Swap<int>(stk.id, handleStack.id);
						chr->getInventory()->set(slot, stk);
						if (handleStack.id == 0) showCursor = true;
						else showCursor = false;
					};
				};
			}
			else if (ev->button.button == SDL_BUTTON_RIGHT)
			{
				int mx, my;
				SDL_GetMouseState(&mx, &my);
				mx -= (leftX+309);
				my -= 150;
				mx /= 26;
				my /= 26;

				if ((mx < 9) && (my < 4) && (mx >= 0) && (my >= 0) && (selection == 1))
				{
					int index = 10 + my * 9 + mx;
					string name = GetPartyMember(xsel);
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
			};
		};
	};
};

void GameMenu::render()
{
	int rightTargetX = SCREEN_WIDTH*48-300;
	if (dying)
	{
		if ((rightX != SCREEN_WIDTH*48) || (leftX != -660))
		{
			if ((Timer::Read()-timer) >= 1)
			{
				timer = Timer::Read();
				if (rightX != SCREEN_WIDTH*48) rightX += 6;
				if (leftX != -660) leftX -= 12;
			};
		};
	}
	else
	{
		if ((rightX != rightTargetX) || (leftX != 0))
		{
			if ((Timer::Read()-timer) >= 1)
			{
				timer = Timer::Read();
				if (rightX != rightTargetX) rightX -= 6;
				if (leftX != 0) leftX += 12;
			};
		};
	};

	ssGameMenuLeft->draw(leftX, 0, 0, false);
	ssGameMenuRight->draw(rightX, 0, 0, false);

	int plotY = 2;
	const char **scan = gameMenuLabels;
	int i = 0;
	while (*scan != NULL)
	{
		int red=255, green=255, blue=255;
		if (i == selection)
		{
			red = 80;
			green = 177;
			blue = 80;

			int plotX = rightX+7;
			if ((Timer::Read() % 1000) < 500) plotX += 2;
			if ((handleStack.id == 0) && (qsel == -1)) ssCursor->draw(plotX, plotY+3, 0, false);
		};

		Text text(*scan, red, green, blue, 255, fntMainMenu);
		text.draw(rightX+28, plotY);

		plotY += 26;
		scan++;
		i++;
	};

	if (selection == 0) drawPartyPanel();
	if (selection == 1) drawInventoryPanel();
	if (selection == 2) drawQuestPanel();
};

bool GameMenu::isLiving()
{
	bool dead = dying && (leftX == -660) && (rightX == SCREEN_WIDTH*48);
	return !dead;
};

void GameMenu::drawPartyMember(int index, int x, int y, int page)
{
	string name = GetPartyMember(index);
	if (name != "")
	{
		Character *chr = GetChar(name);
		stringstream ss;
		ss << "Level " << chr->getLevel();

		chr->getSpriteSheet()->draw(x+10, y+5, 0, false);
		ssElements->draw(x+15, y+50, chr->getElement(), false);
		renderStatusEffectSet(x+58, y+5, chr->getStatusEffectSet());
		Text txt1(chr->getName(), 255, 255, 255);
		Text txt2(ss.str(), 0, 0, 255);

		txt1.draw(x+41, y+50);
		txt2.draw(x+15, y+65);

		if (page == 0)					// Status
		{
			Text txt3("HP", 0, 255, 0);
			Text txt4("MP", 0, 255, 0);
			Text txt5("XP", 0, 255, 0);

			txt3.draw(x+99, y+12);
			txt4.draw(x+99, y+32);
			txt5.draw(x+99, y+52);

			DrawBar(x+124, y+10, chr->getHP(), chr->getMaxHP(), 0);
			DrawBar(x+124, y+30, chr->getMP(), chr->getMaxMP(), 1);
			DrawBar(x+124, y+50, chr->getXP(), chr->getMaxXP(), 2);
		}
		else if (page == 1)				// Stats
		{
			CharStats *stats = chr->getStats();
			CharStats rstats = {0, 0, 0, 0};

			int i;
			for (i=0; i<7; i++)
			{
				ItemStack stack = chr->getInventory()->get(i);
				if (stack.id != 0)
				{
					Item *item = GetItem(stack.id);
					CharStats addstats = {0, 0, 0, 0, 0, 0};
					item->getStat(addstats);

					rstats.STR += addstats.STR;
					rstats.DEF += addstats.DEF;
					rstats.INT += addstats.INT;
					rstats.MDEF += addstats.MDEF;
					rstats.ACC += addstats.ACC;
					rstats.AGI += addstats.AGI;
				};
			};

			StatLabel labels[6] = {
				{"STR", x+100, y+12, &stats->STR, &rstats.STR},
				{"INT", x+300, y+12, &stats->INT, &rstats.INT},
				{"DEF", x+100, y+32, &stats->DEF, &rstats.DEF},
				{"MDEF", x+300, y+32, &stats->MDEF, &rstats.MDEF},
				{"ACC", x+100, y+52, &stats->ACC, &rstats.ACC},
				{"AGI", x+300, y+52, &stats->AGI, &rstats.AGI}
			};

			for (i=0; i<6; i++)
			{
				Text ltxt(labels[i].label, 255, 255, 255, 255);
				ltxt.draw(labels[i].x, labels[i].y);

				stringstream ss;
				ss << *labels[i].aptr;
				Text atxt(ss.str(), 255, 255, 255, 255);
				atxt.draw(labels[i].x+65, labels[i].y);

				ss.str("");
				int rel = *labels[i].rptr;
				if (rel >= 0) ss << "+";
				ss << rel;

				int red=0, green=0, blue=0;
				if (rel < 0)
				{
					red = 255;
				}
				else if (rel == 0)
				{
					blue = 255;
				}
				else
				{
					green = 255;
				};

				Text rtxt(ss.str(), red, green, blue, 255);
				if (rel != 0) rtxt.draw(65+labels[i].x+atxt.getWidth(), labels[i].y);
			};
		}
		else if (page == 2)				// Resistances
		{
			int plotX = x + 100;
			int i;
			int *resistances = chr->getResistances();
			for (i=0; i<Element::NUM_ELEMENTS; i++)
			{
				ssElements->draw(plotX, y+12, i, false);
				stringstream ss;
				int red, green, blue, res;
				res = resistances[i];
				if (res < 0)
				{
					red = 255;
					green = 0;
					blue = 0;
					res = -res;
				}
				else if (res > 100)
				{
					red = 0;
					green = 255;
					blue = 0;
				}
				else
				{
					red = green = blue = 255;
				};
				ss << res;

				Text atxt(ss.str(), red, green, blue, 255);
				atxt.draw(plotX + 24, y+12);
				plotX += 60;
			};
		};
	};
};

void GameMenu::drawPartyPanel()
{
	int i;

	int plotX = leftX+5;
	for (i=0; i<3; i++)
	{
		stringstream ss;
		ss << (i+1);

		Text text(ss.str(), 255, 255, 255, 255);
		text.draw(plotX+12, 430, Text::CENTER);

		plotX += 24;
	};

	Text label(partyPageNames[xsel], 255, 255, 255, 255);
	label.draw(plotX+30, 430);

	int y = 406;
	if ((Timer::Read() % 1000) < 500) y += 2;
	ssCursor->draw(leftX+5+(24*xsel), y, 1, false);

	int plotY = 3;
	for (i=0; i<4; i++)
	{
		drawPartyMember(i, leftX+3, plotY, xsel);
		plotY += 100;
	};
};

void GameMenu::drawInventoryPanel()
{
	int i;
	int plotX = leftX + 309;
	Item *itemSel = NULL;

	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			chr->getSpriteSheet()->draw(plotX, 100, 0, false);

			if (i == xsel)
			{
				int mx, my;
				SDL_GetMouseState(&mx, &my);
				mx -= (leftX+309);
				my -= 150;
				mx /= 26;
				my /= 26;

				int itemSelIndex = my * 9 + mx + 10;
				int px, py;
				Container *cont = chr->getInventory();
				if ((mx < 9) && (my < 4) && (mx >= 0) && (my >= 0))
				{
					ItemStack stackSel = cont->get(itemSelIndex);
					if (stackSel.id != 0)
					{
						itemSel = GetItem(stackSel.id);
					};
				};
				for (px=0; px<9; px++)
				{
					for (py=0; py<4; py++)
					{
						bool sel = (mx == px) && (my == py);
						cont->drawSlot(leftX+309+26*px, 150+26*py, py * 9 + px + 10, sel);
					};
				};

				// Equipment
				SDL_GetMouseState(&mx, &my);
				int rmx = mx;
				mx -= 390;
				mx /= 26;
				int j;
				for (j=0; j<7; j++)
				{
					bool sel = (mx == j) && (my > 330) && (my < 356) && (rmx > 390);
					cont->drawSlot(leftX+390+(26*j), 330, j, sel);

					if (sel)
					{
						ItemStack stk = cont->get(j);
						if (stk.id != 0)
						{
							itemSel = GetItem(stk.id);
						};

						Text txtLabel(getEquipmentLabel(j), 255, 255, 255);
						txtLabel.draw(390, 358);
					};
				};
			};
		};
		plotX += 48;
	};

	drawPartyMember(xsel, leftX+309, 256, 0);

	int plotY = 80;
	if ((Timer::Read() % 1000) < 500) plotY += 2;
	ssCursor->draw(leftX+321+(xsel*48), plotY, 1, false);

	ssInfoPanel->draw(leftX, 0, 0, false);
	Container::drawInfoPanel(leftX, itemSel);

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

	stringstream ss;
	ss << GetPlotState()->gold;

	ssCoin->draw(leftX+585, 268, 0, false);
	Text txtGold(ss.str(), 255, 255, 255, 255);
	txtGold.draw(leftX+609, 268);
};

bool GameMenu::isItemGoodForSlot(int id, int slot)
{
	if (id == 0)
	{
		return slot != 0;
	}
	else
	{
		Item *item = GetItem(id);
		if (item->isStackable()) return false;

		int type = item->getType();
		if ((type == Item::WEAPON)    && (slot == 0)) return true;
		if ((type == Item::SHIELD)    && (slot == 1)) return true;
		if ((type == Item::HELMET)    && (slot == 2)) return true;
		if ((type == Item::ARMOR)     && (slot == 3)) return true;
		if ((type == Item::ACCESSORY) && (slot >= 4) && (slot <= 6)) return true;
		return false;
	};
};

string GameMenu::getEquipmentLabel(int slot)
{
	switch (slot)
	{
	case 0:
		return "Weapon";
	case 1:
		return "Shield";
	case 2:
		return "Helmet";
	case 3:
		return "Armor";
	default:
		return "Accessory";
	};
};

void GameMenu::drawQuestPanel()
{
	Quest *quest = GetQuestList();
	int plotY = 2;
	numQuests = 0;

	while (!quest->isID(""))
	{
		if (quest->getStatus() != Quest::Hidden)
		{
			int index = 0;
			if (quest->getStatus() == Quest::Complete)
			{
				index = 1;
			};

			ssQuest->draw(leftX+24, plotY, index, false);

			int red=255, green=255, blue=255;
			if (numQuests == qsel)
			{
				int x = 2;
				if ((Timer::Read() % 1000) < 500) x += 2;
				ssCursor->draw(leftX+x, plotY, 0, false);

				Text txtDesc(quest->getDescription(), 255, 255, 255, 255, fntText, 400);
				txtDesc.draw(leftX+250, 2);

				red = 0;
				green = 127;
				blue = 255;
			};

			Text text(quest->getTitle(), red, green, blue, 255);
			text.draw(leftX+48, plotY+2);

			plotY += 24;
			numQuests++;
		};

		quest++;
	};
};
