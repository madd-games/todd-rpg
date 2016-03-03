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
 * MainMenu.cpp
 * Main menu.
 */

#include "MainMenu.h"
#include "SpriteSheet.h"
#include "Text.h"
#include "Timer.h"
#include <iostream>
#include "Todd.h"
#include "GameState.h"
#include "SceneView.h"
#include "Mob.h"
#include "LoadView.h"
#include "Options.h"
#include "OptionsView.h"

using namespace std;

MainMenu mainMenu;

#define	MENU_X				600
#define	MENU_Y				100
#define	HOR_SPACING			24

struct MainMenuEntry
{
	const char *label;
	void (*callback)(void);
};

void MainMenuNewGame()
{
	NewGame();
	currentView = &sceneView;
};

void MainMenuLoadGame()
{
	loadView.init();
	currentView = &loadView;
};

void MainMenuOptions()
{
	optionsView.init();
	currentView = &optionsView;
};

void MainMenuQuitGame()
{
	toddQuit = true;
};

#define	NUM_ENTRIES	4
MainMenuEntry mainMenuEntries[NUM_ENTRIES] = {
	{"New Game", MainMenuNewGame},
	{"Load Game", MainMenuLoadGame},
	{"Options", MainMenuOptions},
	{"Quit", MainMenuQuitGame}
};

MainMenu::MainMenu() : selection(0)
{
};

void MainMenu::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_UP)
		{
			if (selection != 0) selection--;
		}
		else if (ev->key.keysym.sym == SDLK_DOWN)
		{
			if (selection != (NUM_ENTRIES-1)) selection++;
		}
		else if ((ev->key.keysym.sym == SDLK_RIGHT) || (ev->key.keysym.sym == options.confirmKey))
		{
			void (*callback)(void) = mainMenuEntries[selection].callback;
			if (callback != NULL)
			{
				callback();
			};
		};
	};
};

void MainMenu::render()
{
	ssMainMenu->draw(0, 0, 0, false);

	int i;
	for (i=0; i<NUM_ENTRIES; i++)
	{
		int red=255, green=255, blue = 255;
		if (i == selection)
		{
			red = 80;
			green = 177;
			blue = 80;
		};
		Text text(mainMenuEntries[i].label, red, green, blue, 255, fntMainMenu);
		text.draw(MENU_X, MENU_Y+HOR_SPACING*i);
	};

	int x = MENU_X-18;
	if ((Timer::Read() % 1000) < 500) x += 2;
	ssCursor->draw(x, 3+MENU_Y+HOR_SPACING*selection, 0, false);
};
