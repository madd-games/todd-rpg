/**
 * GameMenu.cpp
 * The game menu.
 */

#include "GameMenu.h"
#include "Timer.h"
#include "Todd.h"
#include "SpriteSheet.h"
#include "Text.h"

#define	ANIM_TIME		1000

const char *gameMenuLabels[] = {
	"Status",
	"Party",
	"Inventory",
	NULL
};

GameMenu::GameMenu() : rightX(SCREEN_WIDTH*48), leftX(-660), dying(false), selection(0)
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
				if (selection != 0) selection--;
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				selection++;
				if (gameMenuLabels[selection] == NULL) selection--;
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
};

bool GameMenu::isLiving()
{
	bool dead = dying && (leftX == -660) && (rightX == SCREEN_WIDTH*48);
	return !dead;
};
