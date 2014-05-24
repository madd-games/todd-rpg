/**
 * OptionsView.h
 * The screen where a player can configure stuff.
 */

#ifndef OPTIONS_VIEW_H
#define OPTIONS_VIEW_H

#include "View.h"

class OptionsView : public View
{
private:
	int sel;
	bool setting;

public:
	void init();
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
};

extern OptionsView optionsView;

#endif
