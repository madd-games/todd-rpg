/**
 * LootView.h
 * The loot view, where the player can give loot to his party after a battle.
 */

#ifndef LOOT_VIEW_H
#define LOOT_VIEW_H

#include "View.h"
#include <vector>

using namespace std;

class LootView : public View
{
private:
	vector<int> *lootVector;
	int lootIndex;

public:
	void init(vector<int> &lootVector);
	virtual void render();
};

extern LootView lootView;

#endif
