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
 * Mob.cpp
 * A class for handling mobs.
 */

#include "Mob.h"
#include "GameState.h"
#include "SpriteSheet.h"
#include "SceneView.h"
#include <map>
#include "Timer.h"
#include "Todd.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Character.h"
#include <queue>
#include "PlotState.h"
#include "ShopView.h"

using namespace std;

#define	WALK_SPEED			6
#define	WALK_STEPS			8	// Steps per tile
#define	STEP_TIME			45

/**
 * Controls how long we wait before we move auto-mobs around.
 */
unsigned long autoMobTimer;

bool MobState::beginMove(int orient, bool force)
{
	if (steps != 0) return false;

	this->orient = orient;

	offX = 0;
	offY = 0;
	motX = 0;
	motY = 0;

	if (orient == Mob::LEFT)
	{
		if (!sceneView.canWalk(x-1, y) && !force) return false;
		x--;
		offX = 48;
		motX = -WALK_SPEED;
	}
	else if (orient == Mob::RIGHT)
	{
		if (!sceneView.canWalk(x+1, y) && !force) return false;
		x++;
		offX = -48;
		motX = WALK_SPEED;
	}
	else if (orient == Mob::DOWN)
	{
		if (!sceneView.canWalk(x, y+1) && !force) return false;
		y++;
		offY = -48;
		motY = WALK_SPEED;
	}
	else
	{
		if (!sceneView.canWalk(x, y-1) && !force) return false;
		y--;
		offY = 48;
		motY = -WALK_SPEED;
	};

	steps = WALK_STEPS;
	stepTimer = Timer::Read();
	return true;
};

map<string, SpriteSheet*> mobSprites;
map<string, int> mobElements;
map<string, string> mobRealNames;
const char *mobNames[] = {
	"MOBTODD",
	"MOBCASPAR",
	"MOBMANFOREST1",
	"MOBMANFOREST2",
	"MOBBANDIT",
	"MOBMANEASTV1",
	"MOBEASTVPS",
	"MOBDEMSTONE1",
	"MOBBOB",
	"MOBSHPRIEST",
	"MOBSHNECRO",
	NULL
};

// List of steps to be taken by mobs automatically (including non-auto-mobs).
// The 'int' value is the orient.
map<string, queue<int> > mobQueues;

/**
 * Mobs which automatically walk around aimlessly.
 */
const char *autoMobs[] = {
	"MOBMANEASTV1",
	"MOBBOB",
	NULL
};

void InitMobs()
{
	autoMobTimer = Timer::Read();

	// Sprites
	mobSprites["MOBTODD"] =			new SpriteSheet("todd.png");
	mobSprites["MOBCASPAR"] =		new SpriteSheet("caspar.png");
	mobSprites["MOBMANFOREST1"] =		new SpriteSheet("man.png");
	mobSprites["MOBMANFOREST2"] =		mobSprites["MOBMANFOREST1"];
	mobSprites["MOBBANDIT"] =		new SpriteSheet("bandit.png");
	mobSprites["MOBMANEASTV1"] =		mobSprites["MOBMANFOREST1"];
	mobSprites["MOBEASTVPS"] =		mobSprites["MOBMANFOREST1"];
	mobSprites["MOBDEMSTONE1"] =		new SpriteSheet("demonic_stone.png");
	mobSprites["MOBBOB"] =			new SpriteSheet("bob.png");
	mobSprites["MOBSHPRIEST"] =		new SpriteSheet("shadow_priest.png");
	mobSprites["MOBSHNECRO"] = 		new SpriteSheet("shadow_necromancer.png");

	// Names
	mobRealNames["MOBTODD"] =		"Todd";
	mobRealNames["MOBCASPAR"] =		"Caspar";
	mobRealNames["MOBMANFOREST1"] =		"Man";
	mobRealNames["MOBMANFOREST2"] =		"Man";
	mobRealNames["MOBBANDIT"] =		"Bandit";
	mobRealNames["MOBMANEASTV1"] =		"Villager";
	mobRealNames["MOBEASTVPS"] = 		"Eastville Potion Shop Keeper";
	mobRealNames["MOBDEMSTONE1"] = 		"Demonic Stone";
	mobRealNames["MOBBOB"] = 		"Bob";
	mobRealNames["MOBSHPRIEST"] =		"Shadow Priest";
	mobRealNames["MOBSHNECRO"] =		"Shadow Necromancer";

	// Elements
	mobElements["MOBTODD"] =		Element::LIGHT;
	mobElements["MOBCASPAR"] =		Element::FIRE;
	mobElements["MOBMANFOREST1"] =		Element::AIR;
	mobElements["MOBMANFOREST2"] =		Element::WATER;
	mobElements["MOBBANDIT"] =		Element::DARKNESS;
	mobElements["MOBMANEASTV1"] = 		Element::EARTH;
	mobElements["MOBEASTVPS"] =		Element::AIR;
	mobElements["MOBDEMSTONE1"] =		Element::DARKNESS;
	mobElements["MOBBOB"] =			Element::WATER;
	mobElements["MOBSHPRIEST"] =		Element::DARKNESS;
	mobElements["MOBSHNECRO"] = 		Element::DARKNESS;
};

void UpdateMobs()
{
	MobState *stateTodd = (MobState*) GetGameData("MOBTODD", sizeof(MobState));

	const char **scan = autoMobs;
	if ((Timer::Read()-autoMobTimer) >= 1500)
	{
		autoMobTimer = Timer::Read();
		while (*scan != NULL)
		{
			string name(*scan);
			MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
			if ((!state->lock) && (state->sceneID == stateTodd->sceneID))
			{
				srand(time(NULL));
				int chance = rand() % 100;
				if (chance < 40)
				{
					int dir = rand() % 4;
					state->beginMove(dir);
				};
			};
			scan++;
		};
	};

	scan = mobNames;
	while (*scan != NULL)
	{
		string name(*scan);
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));

		if ((state->steps != 0) && (state->sceneID == sceneView.getScene()))
		{
			if ((Timer::Read()-state->stepTimer) >= STEP_TIME)
			{
				state->stepTimer = Timer::Read();
				state->steps--;
				state->offX += state->motX;
				state->offY += state->motY;
			};
		};

		if (state->steps == 0)
		{
			state->offX = 0;
			state->offY = 0;
			state->motX = 0;
			state->motY = 0;

			queue<int> &mq = mobQueues[name];
			if (mq.size() != 0)
			{
				state->beginMove(mq.front(), true);
				mq.pop();
			};
		};

		scan++;
	};
};

void RenderMobs()
{
	MobState *state;

	const char **scan = mobNames;
	while (*scan != NULL)
	{
		string name(*scan);
		state = (MobState*) GetGameData(name, sizeof(MobState));

		if (state->sceneID == sceneView.getScene())
		{
			SpriteSheet *ss = mobSprites[name];
			if (ss != NULL)
			{
				int spriteOff;
				if (state->steps == 0)
				{
					spriteOff = 0;
				}
				else if ((state->steps%2) != 0)
				{
					spriteOff = 1;
				}
				else
				{
					spriteOff = 2;
				};
				ss->draw(state->x*48+state->offX, state->y*48+state->offY, state->orient*3+spriteOff);
			};
		};

		scan++;
	};
};

string GetMobAt(int scene, int x, int y)
{
	const char **scan = mobNames;
	while (*scan != NULL)
	{
		string name(*scan);
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
		if ((state->sceneID == scene) && (state->x == x) && (state->y == y)) return name;

		scan++;
	};

	return "";
};

bool IsMobAt(int scene, int x, int y)
{
	return GetMobAt(scene, x, y) != "";
};

void InteractWithMob(string name)
{
	if (name == "MOBCASPAR")
	{
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
		state->lock = 1;
		sceneView.openDialog(dialCaspar, "MOBCASPAR");
	}
	else if (name == "MOBMANFOREST1")
	{
		if (GetPartyMember(1) == "CHRCASPAR")
		{
			sceneView.openDialog(dialManForest1WithCaspar);
		}
		else
		{
			sceneView.openDialog(dialManForest1WithoutCaspar);
		};
	}
	else if (name == "MOBMANFOREST2")
	{
		PlotState *ps = GetPlotState();
		if (!ps->manEastville1Talked)
		{
			sceneView.openDialog(dialManForest2);
		}
		else
		{
			sceneView.openDialog(dialManForest2b);
		};
	}
	else if (name == "MOBBANDIT")
	{
		sceneView.openDialog(dialBanditBoss);
	}
	else if (name == "MOBMANEASTV1")
	{
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
		state->lock = 1;

		PlotState *ps = GetPlotState();
		if (!ps->manEastville1Talked)
		{
			ps->manEastville1Talked = true;
			sceneView.openDialog(dialManEastville1a, name);
		}
		else
		{
			sceneView.openDialog(dialManEastville1b, name);
		};
	}
	else if (name == "MOBEASTVPS")
	{
		shopView.init("SHOPEASTVPS", "Eastville Potion Shop");
		currentView = &shopView;
	}
	else if (name == "MOBBOB")
	{
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
		state->lock = 1;

		PlotState *ps = GetPlotState();
		if (ps->bobTalkCount == 0)
		{
			ps->bobTalkCount++;
			sceneView.openDialog(dialBob_a, name);
		}
		else
		{
			if (TryTakeAwayItem(Item::GOBLIN_DUST, 10))
			{
				sceneView.openDialog(dialBob_b_yes, name);
			}
			else
			{
				sceneView.openDialog(dialBob_b_no, name);
			};
		};
	}
	else if (name == "MOBDEMSTONE1")
	{
		int i;
		for (i=0; i<4; i++)
		{
			if (GetPartyMember(i) == "CHRBOB") break;
		};

		if (i == 4)
		{
			return;
		};

		Character *chr = GetChar("CHRBOB");
		if (chr->getInventory()->count(Item::SPIRIT_KEY) == 0)
		{
			sceneView.openDialog(dialDemStoneWithoutKey);
		}
		else
		{
			sceneView.openDialog(dialDemStone);
		};
	}
	else if (name == "MOBSHPRIEST")
	{
		sceneView.openDialog(dialShadowPriest);
	}
	else if (name == "MOBSHNECRO")
	{
		sceneView.openDialog(dialShadowNecromancer);
	};
};

MobInfo GetMobInfo(string name)
{
	MobInfo info;
	info.mobSprite = mobSprites[name];
	info.name = mobRealNames[name];
	info.element = mobElements[name];
	return info;
};

bool IsMobReady(string name)
{
	return mobQueues[name].size() == 0;
};

void EnqueueMobMoves(string name, int orient, int count)
{
	while (count--)
	{
		mobQueues[name].push(orient);
	};
};
