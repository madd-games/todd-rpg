/**
 * SaveView.h
 * The "save game" view.
 */

#ifndef LOAD_VIEW_H
#define LOAD_VIEW_H

#include "View.h"
#include <vector>
#include <string>

using namespace std;

class SaveView : public View
{
private:
	vector<string> slotNames;
	int slotSel;
	bool saving;
	string saveName;

public:
	void init();
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
};

extern SaveView saveView;

#endif
