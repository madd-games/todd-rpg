/**
 * ActiveTile.h
 * Handles 'active tiles', such as chests.
 */

#ifndef ACTIVE_TILE_H
#define ACTIVE_TILE_H

#include <stdlib.h>
#include <string>

using namespace std;

struct ChestState
{
	int id;
	int amount;
};

struct DoorState
{
	int targetScene;
	int x;
	int y;
	int orient;
};

class ActiveTile
{
public:
	/**
	 * \brief Returns the size of this tile's state structure.
	 */
	virtual size_t getStateSize() = 0;

	/**
	 * \brief Given the tile state structure, return the tile sprite index to use.
	 */
	virtual int getTileIndex(void *state) = 0;

	/**
	 * \brief This is called when the player presses X while facing this tile.
	 *
	 * Only works with the hard layer.
	 */
	virtual void interact(void *state) = 0;
};

void InitActiveTiles();
ActiveTile *GetActiveTile(int id);
void *GetTileState(int scene, int x, int y, ActiveTile *handler);

#endif
