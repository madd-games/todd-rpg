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
 * SpriteSheet.h
 * A class for managing sprite sheets.
 */

#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

/**
 * A class for representing sprite sheets.
 * Must only be instantiated on the heap (with the 'new' operator).
 */
class SpriteSheet
{
private:
	SDL_Texture *tex;
	int numSprites;
	int spritesPerLine;

	int sprWidth, sprHeight;

public:
	/**
	 * \brief Constuctor.
	 * \param name The name of the data file (PNG) to read the sprite sheet from.
	 * \param width The width of each sprite on the sheet (default is 48).
	 * \param height The height of each sprite on the sheet (default is 48).
	 */
	SpriteSheet(string name, int width = 48, int height = 48);

	/**
	 * \brief Draw a sprite.
	 * \param x The X coordinate to draw on.
	 * \param y The Y coordinate to draw on.
	 * \param rel If this is true, then (x, y) are relative to the camera; otherwise, they are absolute.
	 * \param width The width of the sprite to take (may be less than the real one).
	 * \param dstwidth The destination width (if different from width, then the sprite is scaled).
	 * \param height The height of the sprite to take (may be less than the real one).
	 * \param dstheight The destination height (if different from height, then the sprite is scaled).
	 */
	void draw(int x, int y, int index, bool rel = true, int width = 0, int dstwidth = 0, int height = 0, int dstheight = 0);
};

extern SpriteSheet *ssTiles;
extern SpriteSheet *ssDialog;
extern SpriteSheet *ssMainMenu;
extern SpriteSheet *ssCursor;
extern SpriteSheet *ssElements;
extern SpriteSheet *ssGameMenuRight;
extern SpriteSheet *ssGameMenuLeft;
extern SpriteSheet *ssBars;
extern SpriteSheet *ssSlot;
extern SpriteSheet *ssItems;
extern SpriteSheet *ssInfoPanel;
extern SpriteSheet *ssChestGUI;
extern SpriteSheet *ssBattleMenu;
extern SpriteSheet *ssSkillMenu;
extern SpriteSheet *ssSpark;
extern SpriteSheet *ssOverworld;
extern SpriteSheet *ssMapSpot;
extern SpriteSheet *ssGoblin;
extern SpriteSheet *ssForestBackground;
extern SpriteSheet *ssFlame;
extern SpriteSheet *ssQuest;
extern SpriteSheet *ssCoin;
extern SpriteSheet *ssWaterDrop;
extern SpriteSheet *ssPoison;
extern SpriteSheet *ssStatus;
extern SpriteSheet *ssShadowWarrior;
extern SpriteSheet *ssShadowRealmBackground;
extern SpriteSheet *ssBattleTurn;
extern SpriteSheet *ssShadowTempleBackground;
extern SpriteSheet *ssVampire;
extern SpriteSheet *ssBlood;

#endif
