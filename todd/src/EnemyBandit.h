/**
 * EnemyBandit.h
 */

#ifndef ENEMY_BANDIT_H
#define ENEMY_BANDIT_H

#include "Enemy.h"

class EnemyBandit : public Enemy
{
private:
	int numPotions;
	int numFireSlash;

public:
	EnemyBandit();
	virtual Skill *plan();
	virtual void dropItems(vector<int> &drops);
};

#endif
