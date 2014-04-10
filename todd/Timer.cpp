/**
 * Timer.cpp
 * Synchronized timer.
 */

#include "Timer.h"
#include <SDL2/SDL.h>

unsigned long Timer::state;

void Timer::Update()
{
	state = (unsigned long) SDL_GetTicks();
};

unsigned long Timer::Read()
{
	return state;
};
