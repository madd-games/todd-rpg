/**
 * ChestGUI.h
 * The chest GUI.
 */

#ifndef CHEST_GUI_H
#define CHEST_GUI_H

#include "GUI.h"
#include "ActiveTile.h"

class ChestGUI : public GUI
{
private:
	int id;
	int amount;
	int sel;

public:
	ChestGUI(ChestState *state);
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
	virtual bool isLiving();
};

#endif
