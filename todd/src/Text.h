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
extern TTF_Font *fntBattleCaption;

/**
 * \brief A class for rendering text.
 */
class Text
{
private:
	SDL_Texture *tex;

public:
	/**
	 * \brief An enumeration for specifying text alignment.
	 */
	enum {LEFT, CENTER, RIGHT, TOP, BOTTOM};

	/**
	 * \brief Called when the engine loads.
	 */
	static void Init();

	/**
	 * \brief Constructor.
	 * \param text The text that will be drawn.
	 * \param red The red color component (out of 255).
	 * \param green The green color component (out of 255).
	 * \param blue The blue color component (out of 255).
	 * \param alpha The opacitiy (255 = opaque, 0 = transparent).
	 * \param fnt The font to use.
	 * \param wrap Maximum width of the text; if this is exceeded, the text will wrap.
	 */
	Text(string text, int red = 0, int green = 0, int blue = 0, int alpha = 255, TTF_Font *fnt = fntText, int wrap = 0);
	~Text();

	/**
	 * \brief Draw the text.
	 * \param x The X coordinate on the screen.
	 * \param y The Y coordinate on the screen.
	 * \param xalign Horizontal alignment - Text::LEFT, Text::CENTER or Text::RIGHT.
	 * \param yalign Vertical alignment - Text::TOP, Text::CENTER or Text::BOTTOM.
	 */
	void draw(int x, int y, int xalign = LEFT, int yalign = TOP);

	/**
	 * \brief Return the width of the text.
	 */
	int getWidth();

	/**
	 * \brief Return the height of the text.
	 */
	int getHeight();
};

#endif
