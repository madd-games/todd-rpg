/**
 * Todd.h
 * Include file for the main program; this includes some basic procedures.
 */

#ifndef TODD_H
#define TODD_H

#include <string>
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

void DrawBar(int x, int y, int value, int max, int index);

#endif
