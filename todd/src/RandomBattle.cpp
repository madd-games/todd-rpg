/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014-2016, Madd Games.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer. 
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===============================================================================================
*/

/**
 * RandomBattle.cpp
 * Handling random battles.
 */

#include "RandomBattle.h"
#include "Scene.h"
#include "EnemyGoblin.h"
#include "EnemyShadowWarrior.h"
#include "EnemyVampire.h"
#include <stdlib.h>
#include <time.h>
#include "Todd.h"

bool GetRandomBattle(int sceneID, RandomBattle &bat)
{
	if (sceneID == Scene::Forest)
	{
		if (Probably(10))
		{
			bat.a = new EnemyGoblin;
			bat.b = new EnemyGoblin;
			bat.c = new EnemyGoblin;
			bat.d = new EnemyGoblin;
			return true;
		}
		else if (Probably(20))
		{
			bat.a = new EnemyGoblin;
			bat.b = new EnemyGoblin;
			bat.c = new EnemyGoblin;
			bat.d = NULL;
			return true;
		};
	}
	else if (sceneID == Scene::ShadowRealm)
	{
		if (Probably(20))
		{
			bat.a = new EnemyShadowWarrior;
			bat.b = new EnemyVampire;
			bat.c = new EnemyShadowWarrior;
			bat.d = NULL;
			return true;
		}
		else if (Probably(20))
		{
			bat.a = new EnemyVampire;
			bat.b = new EnemyVampire;
			bat.c = NULL;
			bat.d = NULL;
			return true;
		};
	}
	else if (sceneID == Scene::ShadowTemple)
	{
		if (Probably(10))
		{
			bat.a = new EnemyShadowWarrior;
			bat.b = new EnemyVampire;
			bat.c = new EnemyShadowWarrior;
			bat.d = new EnemyVampire;
			return true;
		}
		else if (Probably(10))
		{
			bat.a = new EnemyVampire;
			bat.b = new EnemyShadowWarrior;
			bat.c = NULL;
			bat.d = NULL;
			return true;
		}
		else if (Probably(5))
		{
			bat.a = new EnemyShadowWarrior;
			bat.b = NULL;
			bat.c = NULL;
			bat.d = NULL;
			return true;
		};
	};

	return false;
};
