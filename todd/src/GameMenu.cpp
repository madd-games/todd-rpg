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

using namespace std;

#define	ANIM_TIME		1000

const char *gameMenuLabels[] = {
	"Status",
	"Inventory",
	NULL
};

GameMenu::GameMenu() : rightX(SCREEN_WIDTH*48), leftX(-660), dying(false), selection(0), xsel(0)
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
			if ((ev->key.keysym.sym == SDLK_ESCAPE) || (ev->key.keysym.sym == SDLK_z))
			{
				timer = Timer::Read();
				dying = true;
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (handleStack.id == 0)
				{
					if (selection != 0)
					{
						selection--;
						xsel = 0;
					};
				};
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				if (handleStack.id == 0)
				{
					selection++;
					if (gameMenuLabels[selection] == NULL) selection--;
					else xsel = 0;
				};
			}
			else if (ev->key.keysym.sym == SDLK_LEFT)
			{
				if (xsel != 0) xsel--;
			}
			else if (ev->key.keysym.sym == SDLK_RIGHT)
			{
				int limit = 0;
				if (selection == 1)		// Inventory
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
							chr->dealDirectDamage(item->getDamage());

							handleStack.amount--;
							if (handleStack.amount == 0) handleStack.id = 0;
							if (handleStack.id == 0) showCursor = true;
							else showCursor = false;
						};
					};
				}
				else if ((rmx > (leftX+390)) && (rmx < (leftX+546)) && (rmy > 330) && (rmy < 356))
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
			if (handleStack.id == 0) ssCursor->draw(plotX, plotY+3, 0, false);
		};

		Text text(*scan, red, green, blue, 255, fntMainMenu);
		text.draw(rightX+28, plotY);

		plotY += 26;
		scan++;
		i++;
	};

	if (selection == 0) drawPartyPanel();
	if (selection == 1) drawInventoryPanel();
};

bool GameMenu::isLiving()
{
	bool dead = dying && (leftX == -660) && (rightX == SCREEN_WIDTH*48);
	return !dead;
};

void GameMenu::drawPartyMember(int index, int x, int y)
{
	string name = GetPartyMember(index);
	if (name != "")
	{
		Character *chr = GetChar(name);
		stringstream ss;
		ss << "Level " << chr->getLevel();

		Text txt1("HP", 0, 255, 0);
		Text txt2("MP", 0, 255, 0);
		Text txt3("XP", 0, 255, 0);
		Text txt4(chr->getName(), 255, 255, 255);
		Text txt5(ss.str(), 0, 0, 255);

		txt1.draw(x+99, y+12);
		txt2.draw(x+99, y+32);
		txt3.draw(x+99, y+52);
		txt4.draw(x+41, y+50);
		txt5.draw(x+15, y+65);

		DrawBar(x+124, y+10, chr->getHP(), chr->getMaxHP(), 0);
		DrawBar(x+124, y+30, chr->getMP(), chr->getMaxMP(), 1);
		DrawBar(x+124, y+50, chr->getXP(), chr->getMaxXP(), 2);

		chr->getSpriteSheet()->draw(x+10, y+5, 0, false);
		ssElements->draw(x+15, y+50, chr->getElement(), false);
	};
};

void GameMenu::drawPartyPanel()
{
	int i;
	int plotY = 3;
	for (i=0; i<4; i++)
	{
		drawPartyMember(i, leftX+3, plotY);
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
					};
				};
			};
		};
		plotX += 48;
	};

	drawPartyMember(xsel, leftX+309, 256);

	int plotY = 80;
	if ((Timer::Read() % 1000) < 500) plotY += 2;
	ssCursor->draw(leftX+321+(xsel*48), plotY, 1, false);

	ssInfoPanel->draw(leftX, 0, 0, false);
	if (itemSel != NULL)
	{
		ssElements->draw(leftX+2, 5, itemSel->getElement(), false);
		ssItems->draw(leftX+28, 5, itemSel->id, false);
		Text txtName(itemSel->getName(), 255, 255, 255, 255, fntItemName);
		txtName.draw(leftX+54, 5);
		Text txtDesc(itemSel->getDesc(), 255, 255, 255, 255, fntText, 300);
		txtDesc.draw(leftX+2, 34);

		int drawY = 350;
		CharStats stats;
		memset(&stats, 0, sizeof(CharStats));
		itemSel->getStat(stats);

		drawY += drawItemStatInfo("STR", stats.STR, drawY);
		drawY += drawItemStatInfo("INT", stats.INT, drawY);
		drawY += drawItemStatInfo("DEF", stats.DEF, drawY);
		drawY += drawItemStatInfo("MDEF", stats.MDEF, drawY);
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
			text.draw(x-1, y+3);
		};
	};
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

int GameMenu::drawItemStatInfo(string statName, int value, int y)
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
	(void)red;
	(void)green;
	stringstream ss;
	if (value > 0) ss << "+";
	ss << value;
	Text txtValue(ss.str(), red, green, 0, 255, fntText);
	txtValue.draw(leftX+50, y);

	return 18;
};
