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
 * GameMenu.h
 * The menu that comes up if you press ESC.
 */

#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "GUI.h"
#include "Item.h"

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
	int qsel;
	int numQuests;

	void drawPartyMember(int index, int x, int y, int page);
	void drawPartyPanel();
	void drawInventoryPanel();
	void drawQuestPanel();

	/**
	 * \brief In character's inventories, slots 0-9 have special use.
	 *
	 * This function returns whether item @id can be placed in slot @slot.
	 */
	bool isItemGoodForSlot(int id, int slot);

	/**
	 * \brief Returns a label for an equipment slot.
	 */
	string getEquipmentLabel(int slot);

	ItemStack handleStack;

public:
	GameMenu();
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
	virtual bool isLiving();
};

#endif
