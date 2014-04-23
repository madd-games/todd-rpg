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
	// TODO: slot selection
	NewGame();
	LoadGame(3);

	MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	sceneView.setScene(state->sceneID);
	currentView = &sceneView;
};

void MainMenuQuitGame()
{
	toddQuit = true;
};

#define	NUM_ENTRIES	3
MainMenuEntry mainMenuEntries[NUM_ENTRIES] = {
	{"New Game", MainMenuNewGame},
	{"Load Game", MainMenuLoadGame},
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
		else if ((ev->key.keysym.sym == SDLK_RIGHT) || (ev->key.keysym.sym == SDLK_x))
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
