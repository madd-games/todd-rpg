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

#define	ANIM_TIME		1000

const char *gameMenuLabels[] = {
	"Status",
	"Inventory",
	NULL
};

GameMenu::GameMenu() : rightX(SCREEN_WIDTH*48), leftX(-660), dying(false), selection(0), xsel(0)
{
	timer = Timer::Read();
};

void GameMenu::handleEvent(SDL_Event *ev)
{
	if (!dying)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == SDLK_ESCAPE)
			{
				timer = Timer::Read();
				dying = true;
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (selection != 0)
				{
					selection--;
					xsel = 0;
				};
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				selection++;
				if (gameMenuLabels[selection] == NULL) selection--;
				else xsel = 0;
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
			ssCursor->draw(plotX, plotY+3, 0, false);
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

		txt1.draw(x+75, y+12);
		txt2.draw(x+75, y+32);
		txt3.draw(x+75, y+52);
		txt4.draw(x+15, y+50);
		txt5.draw(x+15, y+65);

		DrawBar(x+100, y+10, chr->getHP(), chr->getMaxHP(), 0);
		DrawBar(x+100, y+30, chr->getMP(), chr->getMaxMP(), 1);
		DrawBar(x+100, y+50, chr->getXP(), chr->getMaxXP(), 2);

		chr->getSpriteSheet()->draw(x+10, y+5, 0, false);
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
	int plotX = leftX + 209;

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
				mx -= (leftX+209);
				my -= 150;
				mx /= 50;
				my /= 50;

				int px, py;
				Container *cont = chr->getInventory();
				for (px=0; px<9; px++)
				{
					for (py=0; py<4; py++)
					{
						bool sel = (mx == px) && (my == py);
						cont->drawSlot(leftX+209+50*px, 150+50*py, py * 9 + px + 10, sel);
					};
				};
			};
		};
		plotX += 48;
	};

	int plotY = 80;
	if ((Timer::Read() % 1000) < 500) plotY += 2;
	ssCursor->draw(leftX+221+(xsel*48), plotY, 1, false);
};
