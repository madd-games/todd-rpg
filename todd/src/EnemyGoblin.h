/**
 * EnemyGoblin.h
 */

#ifndef ENEMY_GOBLIN_H
#define ENEMY_GOBLIN_H

#include "Enemy.h"

class EnemyGoblin : public Enemy
{
public:
	EnemyGoblin();
	virtual Skill *plan();
};

#endif
