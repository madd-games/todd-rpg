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
	/**
	 * \brief The text to display for this option.
	 */
	const char *text;

	/**
	 * If the user chooses this option, and presses X, this dialog shall appear. This can be NULL,
	 * in which case the dialog is dismissed.
	 */
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
	 * Number of options to choose from, if this is option mode (text == NULL).
	 */
	int numOptions;

	/**
	 * If this is option mode (text == NULL), then these are the options the player
	 * can choose from, and what dialog they lead to.
	 */
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
extern DialogEntry dialManForest2[];
extern DialogEntry dialBanditBoss[];

#endif
