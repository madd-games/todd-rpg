/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014, Madd Games.
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
 * Text.cpp
 * A class for rendering text.
 */

#include "Text.h"
#include "Todd.h"

TTF_Font *fntCaption;
TTF_Font *fntText;
TTF_Font *fntMainMenu;
TTF_Font *fntItemName;
TTF_Font *fntItemCount;
TTF_Font *fntBattleCaption;

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
	fntItemName = LoadFont("bold.ttf", 24);
	fntItemCount = LoadFont("reg.ttf", 8);
	fntBattleCaption = LoadFont("bold.ttf", 24);
};

Text::Text(string text, int red, int green, int blue, int alpha, TTF_Font *fnt, int wrap)
{
	if (text.size() == 0)
	{
		tex = NULL;
		return;
	};

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
	if (tex != NULL) SDL_DestroyTexture(tex);
};

void Text::draw(int x, int y, int xalign, int yalign)
{
	if (tex == NULL) return;

	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);

	switch (xalign)
	{
	case LEFT:
		break;
	case RIGHT:
		x -= w;
		break;
	case CENTER:
		x -= w/2;
		break;
	};

	switch (yalign)
	{
	case TOP:
		break;
	case CENTER:
		y -= h/2;
		break;
	case BOTTOM:
		y -= h;
		break;
	};

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_RenderCopy(sdlRender, tex, NULL, &rect);
};

int Text::getWidth()
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	return w;
};

int Text::getHeight()
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	return h;
};
