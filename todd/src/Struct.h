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
 * Struct.h
 * Handling structures, such as houses and caves
 */

#ifndef STRUCT_H
#define STRUCT_H

#include "SpriteSheet.h"
#include <string>

using namespace std;

/**
 * \brief A class for describing structures, like houses and caves.
 *
 * A structure takes up multiple tiles on the scene grid. Some of the tiles taken up
 * by the structure can be walked on (and, for example, warp), whereas other can't.
 * A grid of bools, known as the mask, indicates which of those tiles can be walked on.
 *
 * A single image file is used for the entire structure, and its size must be divisible by
 * 48, so that it fits on the scene grid.
 */
class Struct
{
private:
	SpriteSheet img;
	int width;
	int height;
	bool *mask;

public:
	/**
	 * \brief Constructor.
	 * \param imgName Name of image file (PNG) that stores the structure graphic.
	 * \param width Width (in grid units) of the image.
	 * \param height Height (in grid units) of the image.
	 * \param mask A grid of 'true' and 'false', where 'true' indicates the tile cannot be
	 *             walked on. Index = (y * width + x). If this is NULL, the mask is assumed to
	 *             be all true.
	 */
	Struct(string imgName, int width, int height, bool *mask);

	/**
	 * \brief Check if the specified location can be walked on.
	 * \param x The X coordinate.
	 * \param y The Y coordinate.
	 * \return true if a mob can walk on the specified tile.
	 */
	bool canWalkOn(int x, int y);

	/**
	 * \brief Draw this structure.
	 * \param x The X coordinate (in pixels!)
	 * \param y The Y coordiante (in pixels!)
	 */
	void draw(int x, int y);

	/**
	 * \brief Return the width of the structure.
	 */
	int getWidth();

	/**
	 * \brief Return the height of the structure.
	 */
	int getHeight();
};

Struct *GetStruct(string name);
void InitStruct();

#endif
