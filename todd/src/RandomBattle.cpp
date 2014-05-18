/**
 * RandomBattle.cpp
 * Handling random battles.
 */

#include "RandomBattle.h"
#include "Scene.h"
#include "EnemyGoblin.h"
#include <stdlib.h>
#include <time.h>

bool GetRandomBattle(int sceneID, RandomBattle &bat)
{
	srand(time(NULL));
	int per = rand() % 100;
	(void)per;

	if (sceneID == Scene::Forest)
	{
		if (per < 10)
		{
			bat.a = new EnemyGoblin;
			bat.b = new EnemyGoblin;
			bat.c = new EnemyGoblin;
			bat.d = new EnemyGoblin;
			return true;
		}
		else if (per < 20)
		{
			bat.a = new EnemyGoblin;
			bat.b = new EnemyGoblin;
			bat.c = new EnemyGoblin;
			bat.d = NULL;
			return true;
		};
	};

	return false;
};
