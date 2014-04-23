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

using namespace std;

#ifdef main
#undef main
#endif

#ifdef _WIN32
#include <windows.h>
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
	MessageBox(NULL, msg.c_str(), "Abort", MB_OK | MB_ICONERROR);
#else
	cerr << "ABORT: " << msg << endl;
#endif

	SDL_Quit();
	exit(1);
};

string GetResourcePath(string name)
{
	string path;
#ifdef _WIN32
	path = getenv("APPDATA");
	path += "\\madd.todd-data\\";
#else
	path = "/usr/share/madd.todd/";
#endif

	return path + name;
};

bool toddQuit = false;
bool showCursor = true;
int main()
{
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

	SDL_Window *win = SDL_CreateWindow("Todd", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH*48, SCREEN_HEIGHT*48, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		GameAbort(string("SDL_CreateWindow() failed: ") + SDL_GetError());
		return 1;
	};

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, /*SDL_RENDERER_ACCELERATED |*/ SDL_RENDERER_PRESENTVSYNC);
	sdlRender = ren;
	if (ren == NULL)
	{
		SDL_DestroyWindow(win);
		GameAbort(string("SDL_CreateRenderer() failed: ") + SDL_GetError());
		return 1;
	};

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

	InitMobs();
	InitItems();
	InitActiveTiles();
	InitSkillsets();
	Text::Init();

	Scene::LoadScenes();
	currentView = &mainMenu;
	sceneView.setScene(Scene::Test);

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
