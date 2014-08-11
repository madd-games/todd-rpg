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
 * DebugOptionsView.cpp
 * A special view available only in debug mode (configure with --debug), which
 * opens when you press F6 in the scene view. You can configure various special
 * options here, such as 'ghost walk' (the ability to walk through objects), and
 * disabling random battles.
 */

#ifdef TODD_DEBUG

#include "DebugOptionsView.h"
#include "SceneView.h"
#include "Todd.h"
#include "Text.h"
#include "SpriteSheet.h"
#include "Timer.h"
#include "Character.h"
#include "Options.h"
#include "PlotState.h"

DebugOptionsView debugOptionsView;

struct DebugOption
{
	const char *label;
	bool *optPtr;
	void (*callback)(void);
};

void OverpowerStats()
{
	int i;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			chr->setHP(99999, 99999);
			chr->setMP(99999, 99999);
			CharStats *stats = chr->getStats();
			stats->STR = 99999;
			stats->DEF = 100;
			stats->INT = 99999;
			stats->MDEF = 100;
			chr->setLevel(99999);
		};
	};
};

void MillionGold()
{
	GetPlotState()->gold = 1000000;
};

DebugOption debugOptions[] = {
	{"Ghost Walk Mode", &sceneView.ghostWalk},
	{"Random Battles", &sceneView.enableRandomBattles},
	{"Overpower Stats", NULL, OverpowerStats},
	{"Million Gold", NULL, MillionGold},
	{NULL, NULL}
};

DebugOptionsView::DebugOptionsView() : sel(0)
{
};

void DebugOptionsView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if ((ev->key.keysym.sym == options.cancelKey) || (ev->key.keysym.sym == SDLK_LEFT) || (ev->key.keysym.sym == SDLK_F6))
		{
			currentView = &sceneView;
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			if (sel != 0) sel--;
		}
		else if (ev->key.keysym.sym == SDLK_DOWN)
		{
			sel++;
			if (debugOptions[sel].label == NULL) sel--;
		}
		else if ((ev->key.keysym.sym == SDLK_RIGHT) || (ev->key.keysym.sym == options.confirmKey))
		{
			bool *ptr = debugOptions[sel].optPtr;
			if (ptr != NULL) *ptr = !(*ptr);
			if (debugOptions[sel].callback != NULL) debugOptions[sel].callback();
		};
	};
};

void DebugOptionsView::render()
{
	int i=0, plotY=2;
	DebugOption *opt = debugOptions;
	while (opt->label != NULL)
	{
		int red=255, green=255, blue=255;
		if (sel == i)
		{
			red = 0;
			green = 127;
			blue = 255;
			int x = 2;
			if ((Timer::Read() % 1000) < 500) x += 2;
			ssCursor->draw(x, plotY, 0, false);
		};

		Text txt1(opt->label, red, green, blue);
		txt1.draw(26, plotY);

		if (opt->optPtr != NULL)
		{
			bool value = *opt->optPtr;
			string word;
			if (value)
			{
				word = "ON";
				red = 0;
				green = 255;
				blue = 0;
			}
			else
			{
				word = "OFF";
				red = 255;
				green = 0;
				blue = 0;
			};

			Text txt2(word, red, green, blue);
			txt2.draw(920, plotY);
		};

		opt++;
		i++;
		plotY += 24;
	};
};

#endif		// TODD_DEBUG
