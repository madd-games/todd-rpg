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
 * Todd.cpp
 * Main source file of the game.
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <sstream>
#include <stdlib.h>
#include "Todd.h"
#include "SpriteSheet.h"
#include "View.h"
#include "SceneView.h"
#include "Scene.h"
#include "Mob.h"
#include "GameState.h"
#include "Timer.h"
#include "Text.h"
#include "MainMenu.h"
#include "Item.h"
#include "ActiveTile.h"
#include "Enemy.h"
#include "Skillset.h"
#include "OverworldView.h"
#include "Options.h"
#include "Struct.h"

using namespace std;

/**
 * Get rid of SDL_main in anything other than Visual Studio.
 */
#ifndef _MSC_VER
#	ifdef main
#		undef main
#	endif
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef TODD_DEBUG
#define TODD_WINDOW_CAPTION "Todd [DEBUG]"
#else
#define TODD_WINDOW_CAPTION "Todd"
#endif

int cameraX = 0, cameraY = 0;
SDL_Renderer *sdlRender;
View *currentView = NULL;

void GameAbort_(string msg, string file, int line)
{
	stringstream ss;
	ss << file << ":" << line << ": " << msg;
	GameAbort__(ss.str());
};

void GameAbort__(string msg)
{
#ifdef _WIN32
	MessageBoxA(NULL, msg.c_str(), "Abort", MB_OK | MB_ICONERROR);
#else
	cerr << "ABORT: " << msg << endl;
#endif

	SDL_Quit();
	exit(1);
};

string GetSubdirFor(string ext)
{
	if (ext == "png") return "images";
	if (ext == "ttf") return "fonts";
	if (ext == "scn") return "scenes";

	// Files that can't be sorted go into the "misc" directory.
	return "misc";
};

string GetResourcePath(string name)
{
	string subdir = "misc";
	size_t dotpos = name.rfind(".");
	if (dotpos != name.npos)
	{
		subdir = GetSubdirFor(name.substr(dotpos+1));
	};

	string path;
#ifdef _WIN32
#ifdef TODD_DATA_DIR
	path = string(TODD_DATA_DIR) + "\\" + subdir + "\\";
#else
	path = getenv("APPDATA");
	path += "\\madd.todd-data\\" + subdir + "\\";
#endif
#else
#ifdef TODD_DATA_DIR
	path = string(TODD_DATA_DIR) + "/" + subdir + "/";
#else
	path = "/usr/share/madd.todd/" + subdir + "/";
#endif
#endif

	return path + name;
};

bool toddQuit = false;
bool showCursor = true;
int main(int argc, char *argv[])
{
#ifdef TODD_DEBUG
	if (argc > 1)
	{
		LoadGame(1);

		string opt = argv[1];
		if (opt == "list")
		{
			PrintGameData();
		}
		else if (opt == "remove")
		{
			if (argc != 3)
			{
				cerr << argv[0] << " remove <game-state-segment-name>" << endl;
				return 1;
			};

			string name = argv[2];
			RemoveGameData(name);
			SaveGame(1, "DEBUG SAVE");
			cout << "Removed" << endl;
			return 0;
		}
		else
		{
			cerr << "unknown command" << endl;
			return 1;
		};

		return 0;
	};
#endif

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		GameAbort(string("SDL_Init() failed: ") + SDL_GetError());
		return 1;
	};

	if (TTF_Init() != 0)
	{
		GameAbort(string("TTF_Init() failed: ") + TTF_GetError());
		return 1;
	};

	SDL_Window *win = SDL_CreateWindow(TODD_WINDOW_CAPTION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH*48, SCREEN_HEIGHT*48, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		GameAbort(string("SDL_CreateWindow() failed: ") + SDL_GetError());
		return 1;
	};

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	sdlRender = ren;
	if (ren == NULL)
	{
		SDL_DestroyWindow(win);
		GameAbort(string("SDL_CreateRenderer() failed: ") + SDL_GetError());
		return 1;
	};

	options.save();

	SpriteSheet *ssMouseCursor = new SpriteSheet("mcursor.png", 24, 24);
	ssTiles = new SpriteSheet("tiles.png");
	ssDialog = new SpriteSheet("dialog.png", 960, 200);
	ssMainMenu = new SpriteSheet("mainmenu.png", 960, 480);
	ssCursor = new SpriteSheet("cursor.png", 24, 24);
	ssElements = new SpriteSheet("elements.png", 24, 24);
	ssGameMenuRight = new SpriteSheet("gamemenu_right.png", 300, 480);
	ssGameMenuLeft = new SpriteSheet("gamemenu_left.png", 660, 480);
	ssBars = new SpriteSheet("bars.png", 150, 20);
	ssSlot = new SpriteSheet("slot.png", 26, 26);
	ssItems = new SpriteSheet("items.png", 24, 24);
	ssInfoPanel = new SpriteSheet("infopanel.png", 300, 480);
	ssChestGUI = new SpriteSheet("chestgui.png", 300, 316);
	ssBattleMenu = new SpriteSheet("battlemenu.png", 150, 200);
	ssSkillMenu = new SpriteSheet("skillmenu.png", 600, 480);
	ssSpark = new SpriteSheet("spark.png", 16, 16);
	ssOverworld = new SpriteSheet("overworld.png", 960, 480);
	ssMapSpot = new SpriteSheet("mapspot.png", 8, 8);
	ssMapSel = new SpriteSheet("mapsel.png", 16, 16);
	ssGoblin = new SpriteSheet("goblin.png");
	ssForestBackground = new SpriteSheet("forest.png", 960, 480);
	ssFlame = new SpriteSheet("flame.png", 16, 16);
	ssQuest = new SpriteSheet("quest.png", 24, 24);
	ssCoin = new SpriteSheet("coin.png", 24, 24);
	ssWaterDrop = new SpriteSheet("waterdrop.png", 16, 16);
	ssPoison = new SpriteSheet("poison.png", 16, 16);
	ssStatus = new SpriteSheet("status.png", 16, 16);
	ssShadowWarrior = new SpriteSheet("shadow_warrior.png");
	ssShadowRealmBackground = new SpriteSheet("shadow_realm.png", 960, 480);
	ssBattleTurn = new SpriteSheet("battle_turn.png");
	ssShadowTempleBackground = new SpriteSheet("shadow_temple.png", 960, 480);
	ssVampire = new SpriteSheet("vampire.png");
	ssBlood = new SpriteSheet("blood.png", 16, 16);
	ssWizard = new SpriteSheet("wizard.png");

	InitMobs();
	InitItems();
	InitActiveTiles();
	InitSkillsets();
	InitStruct();
	Text::Init();
	overworldView.init();

	Scene::LoadScenes();
	currentView = &mainMenu;

	NewGame();
	SDL_ShowCursor(SDL_DISABLE);

	SDL_Event e;
	while (!toddQuit)
	{
		Timer::Update();
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				toddQuit = true;
			}
			else
			{
				currentView->handleEvent(&e);
			};
		};

		SDL_RenderClear(ren);
		currentView->render();
		if (showCursor)
		{
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			ssMouseCursor->draw(mx, my, 0, false);
		};
		SDL_RenderPresent(ren);
	};

	SDL_Quit();
	return 0;
};

void DrawBar(int x, int y, int value, int max, int index)
{
	int width = value * 150 / max;
	ssBars->draw(x, y, 3, false);
	if (width != 0) ssBars->draw(x, y, index, false, width);

	stringstream ss;
	ss << value << "/" << max;
	Text text(ss.str(), 255, 255, 255);
	text.draw(x+3, y+2);
};

int RandomUniform(int min, int max)
{
	random_device rd;
	default_random_engine e1(rd());
	uniform_int_distribution<int> mknum(min, max);
	return mknum(e1);
};

bool Probably(int chance)
{
	return RandomUniform(0, 100) < chance;
};
