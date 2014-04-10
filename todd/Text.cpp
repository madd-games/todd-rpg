/**
 * Text.cpp
 * A class for rendering text.
 */

#include "Text.h"
#include "Todd.h"

TTF_Font *fntCaption;
TTF_Font *fntText;
TTF_Font *fntMainMenu;

TTF_Font *LoadFont(string name, int size)
{
	TTF_Font *fnt = TTF_OpenFont(GetResourcePath(name).c_str(), size);
	if (fnt == NULL)
	{
		GameAbort(string("Failed to load font ") + name + ": " + TTF_GetError());
	};
	return fnt;
};

void Text::Init()
{
	fntCaption = LoadFont("bold.ttf", 32);
	fntText = LoadFont("reg.ttf", 16);
	fntMainMenu = LoadFont("bold.ttf", 24);
};

Text::Text(string text, int red, int green, int blue, int alpha, TTF_Font *fnt, int wrap)
{
	SDL_Color color;
	color.r = red;
	color.g = green;
	color.b = blue;
	color.a = alpha;
	SDL_Surface *surf;

	if (wrap == 0)
	{
		surf = TTF_RenderUTF8_Blended(fnt, text.c_str(), color);
		if (surf == NULL)
		{
			GameAbort(string("TTF_RenderUTF8_Blended(): ") + TTF_GetError());
		};
	}
	else
	{
		surf = TTF_RenderUTF8_Blended_Wrapped(fnt, text.c_str(), color, wrap);
		if (surf == NULL)
		{
			GameAbort(string("TTF_RenderUTF8_Blended_Wrapped(): ") + TTF_GetError());
		};
	};

	tex = SDL_CreateTextureFromSurface(sdlRender, surf);
	SDL_FreeSurface(surf);
	if (tex == NULL)
	{
		GameAbort(string("SDL_CreateTextureFromSurface(): ") + SDL_GetError());
	};
};

Text::~Text()
{
	SDL_DestroyTexture(tex);
};

void Text::draw(int x, int y)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_RenderCopy(sdlRender, tex, NULL, &rect);
};
