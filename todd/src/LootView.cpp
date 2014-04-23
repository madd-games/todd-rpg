/**
 * LootView.cpp
 * The loot view, where the player can give loot to his party after a battle.
 */

#include "LootView.h"

LootView lootView;

void LootView::init(vector<int> &lootVector)
{
	this->lootVector = &lootVector;
	lootIndex = 0;
};

void LootView::render()
{
};
