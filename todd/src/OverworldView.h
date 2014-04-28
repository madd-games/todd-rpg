/**
 * OverworldView.h
 * The "overworld map" view.
 */

#ifndef OVERWORLD_VIEW_H
#define OVERWORLD_VIEW_H

#include "View.h"

class OverworldView : public View
{
private:
	int currentSpot;

public:
	void init();
	void setPosition(int id);
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
};

extern OverworldView overworldView;

#endif
