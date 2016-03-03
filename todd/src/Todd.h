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
 * Todd.h
 * Include file for the main program; this includes some basic procedures.
 */

#ifndef TODD_H
#define TODD_H

#include <string>
#include <random>
#include "View.h"

using namespace std;

#define	SCREEN_WIDTH 20
#define	SCREEN_HEIGHT 10

#define	GameAbort(msg)	GameAbort_((msg), __FILE__, __LINE__)
void GameAbort_(string msg, string file, int line);
void GameAbort__(string msg);

string GetResourcePath(string name);

extern SDL_Renderer *sdlRender;
extern int cameraX, cameraY;
extern View *currentView;
extern bool toddQuit;
extern bool showCursor;

void DrawBar(int x, int y, int value, int max, int index);
int RandomUniform(int min, int max);
bool Probably(int chance);

template<class T>
void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
};

#endif
