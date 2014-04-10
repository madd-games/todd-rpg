/**
 * GameMenu.h
 * The menu that comes up if you press ESC.
 */

#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "GUI.h"

class GameMenu : public GUI
{
private:
	unsigned long timer;
	int rightX;
	int leftX;

	bool dying;

	// The menu
	int selection;
	int xsel;

	void drawPartyMember(int index, int x, int y);
	void drawPartyPanel();
	void drawInventoryPanel();

public:
	GameMenu();
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
	virtual bool isLiving();
};

#endif
