/**
 * Dialog.h
 * The "dialog" GUI, which displays people talking.
 */

#ifndef DIALOG_H
#define DIALOG_H

#include "GUI.h"
#include "Element.h"
#include <string>

using namespace std;

/**
 * An array of these defines a dialog.
 */
struct DialogEntry
{
	/**
	 * Who speaks (the mob name)
	 */
	const char *speaker;

	/**
	 * What they're saying
	 */
	const char *text;

	/**
	 * If not NULL, then this function is called right after the user presses X
	 * to dismiss this part of the dialog.
	 */
	void (*callback)(void);
};

class Dialog : public GUI
{
private:
	DialogEntry *currentEntry;
	string with;
	unsigned long timer;
	int letters;

public:
	Dialog(DialogEntry *entry, string with);
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
	virtual bool isLiving();
};

extern DialogEntry dialFeminist[];

#endif
