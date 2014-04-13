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
