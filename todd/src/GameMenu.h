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

	void drawPartyMember(int index, int x, int y);
	void drawPartyPanel();
	void drawInventoryPanel();

	/**
	 * \brief In character's inventories, slots 0-9 have special use.
	 *
	 * This function returns whether item @id can be placed in slot @slot.
	 */
	bool isItemGoodForSlot(int id, int slot);

	/**
	 * \brief Draw the stats part of an item's description.
	 *
	 * Returns the height of the drawn text.
	 */
	int drawItemStatInfo(string statName, int value, int y);

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
