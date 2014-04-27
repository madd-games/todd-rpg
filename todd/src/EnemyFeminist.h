/**
 * EnemyFeminist.h
 * The "Feminist" enemy.
 */

#ifndef ENEMY_FEMINIST_H
#define ENEMY_FEMINIST_H

#include "Enemy.h"

class EnemyFeminist : public Enemy
{
public:
	EnemyFeminist();
	virtual Skill *plan();
	virtual void dropItems(vector<int> &drops);
};

#endif
