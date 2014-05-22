/**
 * Options.h
 * Header file for managing player's options.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <SDL2/SDL.h>

using namespace std;

class Options
{
private:
	string getPathToOptions();

public:
	SDL_Keycode confirmKey;
	SDL_Keycode cancelKey;
	SDL_Keycode upKey;
	SDL_Keycode downKey;
	SDL_Keycode leftKey;
	SDL_Keycode rightKey;
	SDL_Keycode menuKey;

	Options();
	void save();
};

extern Options options;

#endif
