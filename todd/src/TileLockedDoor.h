/**
 * TileLockedDoor.h
 * The 'locked door' active tile.
 */

#include <iostream>
#include "Character.h"
#include "SceneView.h"
#include "Dialog.h"
#include "Mob.h"

using namespace std;

class TileLockedDoor : public ActiveTile
{
public:
	size_t getStateSize()
	{
		return 0;
	};

	int getTileIndex(void *state_)
	{
		(void)state_;
		return 11;
	};

	void interact(void *state_)
	{
		(void)state_;
		sceneView.openDialog(dialLockedDoor);
	};
};
