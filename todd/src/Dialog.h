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

struct DialogEntry;
struct DialogOption
{
	const char *text;
	DialogEntry *effect;
};

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
	 * The caption (usually the display name of the mob)
	 */
	const char *caption;

	/**
	 * What they're saying - NULL will cause the option mode to be applied.
	 */
	const char *text;

	/**
	 * If not NULL, then this function is called right after the user presses X
	 * to dismiss this part of the dialog.
	 */
	void (*callback)(void);

	/**
	 * If this is option mode (text == NULL), then these are the options the player
	 * can choose from, and what dialog they lead to.
	 */
	int numOptions;
	DialogOption options[4];

	/**
	 * If this is not NULL, then it specifies the name of a mob who must complete its
	 * command queue before X can be pressed to dismiss the message.
	 */
	const char *mobwait;
};

class Dialog : public GUI
{
private:
	DialogEntry *currentEntry;
	string with;
	unsigned long timer;
	int letters;
	int choice;

public:
	Dialog(DialogEntry *entry, string with);
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
	virtual bool isLiving();
};

extern DialogEntry dialFeminist[];
extern DialogEntry dialCrystal[];
extern DialogEntry dialLockedDoor[];
extern DialogEntry dialCaspar[];
extern DialogEntry dialManForest1WithoutCaspar[];
extern DialogEntry dialManForest1WithCaspar[];
extern DialogEntry dialBanditBoss[];

#endif
