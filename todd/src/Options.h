/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014-2016, Madd Games.
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
 * Options.h
 * Header file for managing player's options.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <SDL2/SDL.h>

using namespace std;

/**
 * \brief A class that handles options.
 */
class Options
{
private:
	string getPathToOptions();

public:
	/**
	 * \brief The key used to confirm stuff (default is X).
	 */
	SDL_Keycode confirmKey;

	/**
	 * \brief The key used to cancel stuff (default is Z).
	 */
	SDL_Keycode cancelKey;

	/**
	 * \brief The key for walking up (default is up arrow key).
	 */
	SDL_Keycode upKey;

	/**
	 * \brief The key for walking down (default is down arrow key).
	 */
	SDL_Keycode downKey;

	/**
	 * \brief The key for walking left (default is left arrow key).
	 */
	SDL_Keycode leftKey;

	/**
	 * \brief The key for walking right (default is right arrow key).
	 */
	SDL_Keycode rightKey;

	/**
	 * \brief The key for opening the menu (default is ESC).
	 */
	SDL_Keycode menuKey;

	Options();

	/**
	 * \brief Save the current options in the appropriate file.
	 *
	 * The file to which settings are saved and read from is platform-dependent.
	 * It is called `madd.todd-options`, and on Windows it is stored in APPDATA, and on UNIX it's
	 * stored in HOME.
	 */
	void save();
};

extern Options options;

#endif
