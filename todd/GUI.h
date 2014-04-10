/**
 * GUI.h
 * An abstract class that allows for the handling of various GUIs
 * on top of the scene view.
 */

#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

class GUI
{
public:
	virtual void handleEvent(SDL_Event *ev) = 0;
	virtual void render() = 0;

	/**
	 * \brief Returns true if the GUI still needs to be alive.
	 */
	virtual bool isLiving() = 0;
};

#endif
