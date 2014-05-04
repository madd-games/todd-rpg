/**
 * RandomBattle.h
 * Handling random battles.
 */

#ifndef RANDOM_BATTLE_H
#define RANDOM_BATTLE_H

#include "Enemy.h"

struct RandomBattle
{
	Enemy *a;
	Enemy *b;
	Enemy *c;
	Enemy *d;
};

/**
 * \brief Set up random battles.
 *
 * If a random battle was chosen, 'true' is returned, and 'bat' is filled with information
 * about the planned battle.
 */
bool GetRandomBattle(int scene, RandomBattle &bat);

#endif
