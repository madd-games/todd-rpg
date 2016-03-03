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
 * Struct.cpp
 * Handling structures, such as houses and caves
 */

#include "Struct.h"
#include <map>

using namespace std;

map<string, Struct*> structMap;

Struct::Struct(string imgName, int width, int height, bool *mask) : img(imgName, width*48, height*48),
	width(width), height(height), mask(mask)
{
};

bool Struct::canWalkOn(int x, int y)
{
	if (mask == NULL)
	{
		return false;
	}
	else
	{
		return !mask[y * width + x];
	};
};

void Struct::draw(int x, int y)
{
	img.draw(x, y, 0);
};

int Struct::getWidth()
{
	return width;
};

int Struct::getHeight()
{
	return height;
};

Struct *GetStruct(string name)
{
	return structMap[name];
};

void InitStruct()
{
	structMap["h1"] = new Struct("h1.png", 7, 5, NULL);
};
