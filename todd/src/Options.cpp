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
 * Options.cpp
 * Player's options.
 */

#include "Options.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>

using namespace std;

Options options;

Options::Options() :
	confirmKey(SDLK_x),
	cancelKey(SDLK_z),
	upKey(SDLK_UP),
	downKey(SDLK_DOWN),
	leftKey(SDLK_LEFT),
	rightKey(SDLK_RIGHT),
	menuKey(SDLK_ESCAPE)
{
	ifstream ifs(getPathToOptions().c_str(), ios::binary);
	if (!ifs.fail())
	{
		ifs.read((char*) this, sizeof(Options));
		ifs.close();
	};
};

string Options::getPathToOptions()
{
	stringstream ss;
#ifdef _WIN32
	ss << getenv("APPDATA") << "\\madd.todd-options";
#else
	ss << getenv("HOME") << "/madd.todd-options";
#endif
	return ss.str();
};

void Options::save()
{
	ofstream ofs(getPathToOptions().c_str(), ios::binary);
	if (!ofs.fail())
	{
		ofs.write((const char*) this, sizeof(Options));
		ofs.close();
	};
};
