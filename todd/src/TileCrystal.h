/**
 * TileCrystal.h
 * The 'chest' active tile.
 */

#include <iostream>
#include "Character.h"
#include "SceneView.h"
#include "Dialog.h"

using namespace std;

class TileCrystal : public ActiveTile
{
public:
	size_t getStateSize()
	{
		return 0;
	};

	int getTileIndex(void *state_)
	{
		(void)state_;
		return 6;
	};

	void interact(void *state_)
	{
		(void)state_;
		sceneView.openDialog(dialCrystal);
	};
};
