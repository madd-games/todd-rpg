/**
 * TileDoor.h
 * The 'door' active tile.
 */

#include <iostream>
#include "Character.h"
#include "SceneView.h"
#include "Dialog.h"
#include "Mob.h"

using namespace std;

class TileDoor : public ActiveTile
{
public:
	size_t getStateSize()
	{
		return sizeof(DoorState);
	};

	int getTileIndex(void *state_)
	{
		(void)state_;
		return 11;
	};

	void interact(void *state_)
	{
		DoorState *state = (DoorState*) state_;
		MobState *ms = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
		ms->sceneID = state->targetScene;
		ms->x = state->x;
		ms->y = state->y;
		ms->orient = state->orient;
		sceneView.setScene(state->targetScene);
	};
};
