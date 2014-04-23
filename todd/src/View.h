/**
 * View.h
 * An abstract class for different views (e.g. map, screen, etc.)
 */

#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>

class View
{
public:
	virtual void handleEvent(SDL_Event *ev);
	virtual void render() = 0;
};

#endif
