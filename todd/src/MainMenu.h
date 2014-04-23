/**
 * MainMenu.h
 * Main menu.
 */

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "View.h"

class MainMenu : public View
{
private:
	int selection;

public:
	MainMenu();
	void handleEvent(SDL_Event *ev);
	void render();
};

extern MainMenu mainMenu;

#endif
