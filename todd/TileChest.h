/**
 * TileChest.h
 * The 'chest' active tile.
 */

#include <iostream>
#include "Character.h"

using namespace std;

class TileChest : public ActiveTile
{
public:
	size_t getStateSize()
	{
		return sizeof(ChestState);
	};

	int getTileIndex(void *state_)
	{
		ChestState *state = (ChestState*) state_;
		if (state->id == 0) return 5;
		else return 4;
	};

	void interact(void *state_)
	{
		ChestState *state = (ChestState*) state_;
		if (state->id != 0)
		{
			ItemStack istack;
			istack.id = state->id;
			istack.amount = state->amount;
			state->id = state->amount = 0;
			GetChar("CHRTODD")->getInventory()->pushStack(istack);
		};
	};
};
