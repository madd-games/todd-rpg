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
 * SpriteSheet.cpp
 * A class that handles sprite sheets.
 */

#include "SpriteSheet.h"
#include "Todd.h"

SpriteSheet *ssTiles;
SpriteSheet *ssDialog;
SpriteSheet *ssMainMenu;
SpriteSheet *ssCursor;
SpriteSheet *ssElements;
SpriteSheet *ssGameMenuRight;
SpriteSheet *ssGameMenuLeft;
SpriteSheet *ssBars;
SpriteSheet *ssSlot;
SpriteSheet *ssItems;
SpriteSheet *ssInfoPanel;
SpriteSheet *ssChestGUI;
SpriteSheet *ssBattleMenu;
SpriteSheet *ssSkillMenu;
SpriteSheet *ssSpark;
SpriteSheet *ssOverworld;
SpriteSheet *ssMapSpot;
SpriteSheet *ssGoblin;
SpriteSheet *ssForestBackground;
SpriteSheet *ssFlame;
SpriteSheet *ssQuest;
SpriteSheet *ssCoin;

SpriteSheet::SpriteSheet(string name, int width, int height) : sprWidth(width), sprHeight(height)
{
	string path = GetResourcePath(name);
	tex = IMG_LoadTexture(sdlRender, path.c_str());

	if (tex == NULL)
	{
		GameAbort(string("Could not load sprite sheet ") + name + ": IMG_LoadTexture() failed: " + IMG_GetError());
	};

	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);

	if ((w % sprWidth) != 0)
	{
		GameAbort(string("Could not load sprite sheet ") + name + ": width is not divisible by sprite width");
	};

	if ((h % sprHeight) != 0)
	{
		GameAbort(string("Could not load sprite sheet: ") + name + ": height is not divisible by sprite height");
	};

	numSprites = (w / sprWidth) * (h / sprHeight);
	spritesPerLine = w / sprWidth;
};

void SpriteSheet::draw(int x, int y, int index, bool rel, int width, int dstwidth, int height, int dstheight)
{
	if (width == 0) width = sprWidth;
	if (height == 0) height = sprHeight;

	if (dstwidth == 0) dstwidth = width;
	if (dstheight == 0) dstheight = height;

	if (rel)
	{
		x -= cameraX;
		y -= cameraY;
	};
	if (index >= numSprites) return;

	SDL_Rect src;
	SDL_Rect dst;

	src.x = (index % spritesPerLine) * sprWidth;
	src.y = (index / spritesPerLine) * sprHeight;
	src.w = width;
	src.h = height;

	dst.x = x;
	dst.y = y;
	dst.w = dstwidth;
	dst.h = dstheight;

	SDL_RenderCopy(sdlRender, tex, &src, &dst);
};
