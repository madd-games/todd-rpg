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
 * ActiveTile.cpp
 * Handles 'active tiles', such as chests.
 */

#include "ActiveTile.h"
#include <map>
#include <sstream>
#include "GameState.h"

// Include all the active tile classes here.
#include "TileChest.h"
#include "TileCrystal.h"
#include "TileDoor.h"
#include "TileLockedDoor.h"
#include "TileHiddenDoor.h"

using namespace std;

map<int, ActiveTile*> activeTileMap;

void InitActiveTiles()
{
	activeTileMap[4] = new TileChest;
	activeTileMap[5] = new TileCrystal;
	activeTileMap[8] = new TileDoor;
	activeTileMap[9] = new TileLockedDoor;
	activeTileMap[11] = new TileHiddenDoor;
};

ActiveTile *GetActiveTile(int id)
{
	return activeTileMap[id];
};

void *GetTileState(int scene, int x, int y, ActiveTile *handler)
{
	const char hexd[17] = "0123456789ABCDEF";
	stringstream ss;
	ss << "ATI" << hexd[(scene >> 4) & 0xF] << hexd[scene & 0xF]
		<< hexd[(x >> 4) & 0xF] << hexd[x & 0xF]
		<< hexd[(y >> 4) & 0xF] << hexd[y & 0xF];
	return GetGameData(ss.str(), handler->getStateSize());
};
