/**
 * Text.h
 * A class for rendering text.
 */

#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

using namespace std;

extern TTF_Font *fntCaption;
extern TTF_Font *fntText;
extern TTF_Font *fntMainMenu;
extern TTF_Font *fntItemName;
extern TTF_Font *fntItemCount;

class Text
{
private:
	SDL_Texture *tex;

public:
	static void Init();

	Text(string text, int red = 0, int green = 0, int blue = 0, int alpha = 255, TTF_Font *fnt = fntText, int wrap = 0);
	~Text();
	void draw(int x, int y);
};

#endif
