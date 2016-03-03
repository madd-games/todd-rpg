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
 * OptionsView.cpp
 * The screen where a player can configure stuff.
 */

#include "OptionsView.h"
#include "Options.h"
#include <sstream>
#include "Text.h"
#include "SpriteSheet.h"
#include "Timer.h"
#include "MainMenu.h"
#include "Todd.h"

OptionsView optionsView;

// Control option.
typedef struct
{
	const char *name;
	SDL_Keycode *ptr;
} ControlOption;

ControlOption controlOptions[] = {
	{"Walk left key", &options.leftKey},
	{"Walk right key", &options.rightKey},
	{"Walk up key", &options.upKey},
	{"Walk down key", &options.downKey},
	{"Confirm/interact key", &options.confirmKey},
	{"Cancel key", &options.cancelKey},
	{"Game menu key", &options.menuKey},
	{"Save", NULL},
	{NULL, NULL}
};

void OptionsView::init()
{
	sel = 0;
	setting = false;
};

void OptionsView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (setting)
		{
			*controlOptions[sel].ptr = ev->key.keysym.sym;
			setting = false;
		}
		else
		{
			if (ev->key.keysym.sym == SDLK_DOWN)
			{
				sel++;
				if (controlOptions[sel].name == NULL) sel--;
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (sel != 0) sel--;
			}
			else if (ev->key.keysym.sym == options.confirmKey)
			{
				if (controlOptions[sel].ptr == NULL)
				{
					options.save();
					currentView = &mainMenu;
				}
				else
				{
					setting = true;
				};
			};
		};
	};
};

void OptionsView::render()
{
	ControlOption *opt = controlOptions;
	int plotY = 2;
	int i = 0;

	while (opt->name != NULL)
	{
		stringstream ss;
		if (setting && (i == sel))
		{
			ss << opt->name << " [Press key to set]";
		}
		else
		{
			if (opt->ptr != NULL)
			{
				ss << opt->name << " [" << SDL_GetKeyName(*opt->ptr) << "]";
			}
			else
			{
				ss << opt->name;
			};
		};

		int red=255, green=255, blue=255;
		if (sel == i)
		{
			red = 0;
			green = 127;
			int x = 2;
			if ((Timer::Read() % 1000) < 500) x += 2;
			ssCursor->draw(x, plotY, 0, false);
		};
		Text text(ss.str(), red, green, blue, 255);
		text.draw(24, plotY);

		plotY += 24;
		i++;
		opt++;
	};
};
