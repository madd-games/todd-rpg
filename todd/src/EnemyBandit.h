/**
 * EnemyBandit.h
 */

#ifndef ENEMY_BANDIT_H
#define ENEMY_BANDIT_H

#include "Enemy.h"

class EnemyBandit : public Enemy
{
public:
	EnemyBandit();
	virtual Skill *plan();
};

#endif
