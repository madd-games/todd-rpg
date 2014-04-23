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

using namespace std;

map<int, ActiveTile*> activeTileMap;

void InitActiveTiles()
{
	activeTileMap[4] = new TileChest;
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
