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
 * LoadView.cpp
 * The "load game" view.
 */

#include "LoadView.h"
#include "MainMenu.h"
#include "GameState.h"
#include <fstream>
#include <sstream>
#include "Text.h"
#include "Todd.h"
#include "SpriteSheet.h"
#include "Timer.h"
#include "Mob.h"
#include "SceneView.h"
#include "Options.h"

using namespace std;

LoadView loadView;

void LoadView::init()
{
	slotSel = 1;
	slotNames.clear();
	int i;
	for (i=1; i<17; i++)
	{
		string path = GetPathToSaveSlot(i);
		ifstream ifs(path.c_str(), ios::binary);
		if (!ifs.fail())
		{
			SaveHeader head;
			ifs.read((char*)&head, sizeof(SaveHeader));
			ifs.close();

			char buf[33];
			buf[32] = 0;
			memcpy(buf, head.name, 32);

			slotNames.push_back(buf);
		}
		else
		{
			slotNames.push_back("EMPTY SLOT");
		};
	};
};

void LoadView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if ((ev->key.keysym.sym == options.cancelKey) || (ev->key.keysym.sym == SDLK_LEFT))
		{
			currentView = &mainMenu;
		}
		else if (ev->key.keysym.sym == SDLK_DOWN)
		{
			if (slotSel != 16) slotSel++;
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			if (slotSel != 1) slotSel--;
		}
		else if ((ev->key.keysym.sym == SDLK_RIGHT) || (ev->key.keysym.sym == options.confirmKey))
		{
			NewGame();
			LoadGame(slotSel);

			MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
			sceneView.setScene(state->sceneID);
			currentView = &sceneView;
		};
	};
};

void LoadView::render()
{
	Text txtLoadGame("LOAD GAME", 255, 255, 255, 255, fntCaption);
	txtLoadGame.draw(480, 2, Text::CENTER);

	int plotY = 64;
	int i;
	for (i=1; i<17; i++)
	{
		stringstream ss;
		ss << i << ". " << slotNames[i-1];
		Text text(ss.str(), 255, 255, 255, 255, fntText);
		text.draw(24, plotY+2);

		if (i == slotSel)
		{
			int x = 2;
			if ((Timer::Read() % 1000) < 500) x += 2;
			ssCursor->draw(x, plotY, 0, false);
		};

		plotY += 24;
	};
};
