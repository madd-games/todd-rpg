/**
 * DebugOptionsView.h
 * A special view available only in debug mode (configure with --debug), which
 * opens when you press F6 in the scene view. You can configure various special
 * options here, such as 'ghost walk' (the ability to walk through objects), and
 * disabling random battles.
 */

#ifndef TODD_DEBUG
#error Included DebugOptionsView.h in non-debug build!
#endif

#ifndef DEBUG_OPTIONS_VIEW_H
#define DEBUG_OPTIONS_VIEW_H

#include "View.h"

class DebugOptionsView : public View
{
private:
	int sel;

public:
	DebugOptionsView();
	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
};

extern DebugOptionsView debugOptionsView;

#endif
