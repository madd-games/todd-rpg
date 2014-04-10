/**
 * SpriteSheet.h
 * A class for managing sprite sheets.
 */

#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class SpriteSheet
{
private:
	SDL_Texture *tex;
	int numSprites;
	int spritesPerLine;

	int sprWidth, sprHeight;

public:
	SpriteSheet(string name, int width = 48, int height = 48);
	void draw(int x, int y, int index, bool rel = true);
};

extern SpriteSheet *ssTiles;
extern SpriteSheet *ssDialog;
extern SpriteSheet *ssMainMenu;
extern SpriteSheet *ssCursor;
extern SpriteSheet *ssElements;
extern SpriteSheet *ssGameMenuRight;
extern SpriteSheet *ssGameMenuLeft;

#endif
