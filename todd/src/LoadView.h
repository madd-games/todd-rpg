/**
 * LoadView.h
 * The "load game" view.
 */

#ifndef LOAD_VIEW_H
#define LOAD_VIEW_H

#include "View.h"
#include <vector>
#include <string>

using namespace std;

class LoadView : public View
{
private:
	vector<string> slotNames;
	int slotSel;

public:
	void init();
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
};

extern LoadView loadView;

#endif
