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

using namespace std;

/**
 * The magic boundary - the player must be at least this many pixels from the border.
 * If they're not, the camera moves to make them.
 */
#define	MAGIC_BOUND			(48*5)

#define	WALK_SPEED			6
#define	WALK_STEPS			8	// Steps per tile
#define	STEP_TIME			65

/**
 * Controls how long we wait before we move auto-mobs around.
 */
unsigned long autoMobTimer;

void MobState::beginMove(int orient, bool force)
{
	if (steps != 0) return;

	this->orient = orient;

	offX = 0;
	offY = 0;
	motX = 0;
	motY = 0;

	if (orient == Mob::LEFT)
	{
		if (!sceneView.canWalk(x-1, y) && !force) return;
		x--;
		offX = 48;
		motX = -WALK_SPEED;
	}
	else if (orient == Mob::RIGHT)
	{
		if (!sceneView.canWalk(x+1, y) && !force) return;
		x++;
		offX = -48;
		motX = WALK_SPEED;
	}
	else if (orient == Mob::DOWN)
	{
		if (!sceneView.canWalk(x, y+1) && !force) return;
		y++;
		offY = -48;
		motY = WALK_SPEED;
	}
	else
	{
		if (!sceneView.canWalk(x, y-1) && !force) return;
		y--;
		offY = 48;
		motY = -WALK_SPEED;
	};

	steps = WALK_STEPS;
	stepTimer = Timer::Read();
};

map<string, SpriteSheet*> mobSprites;
map<string, int> mobElements;
map<string, string> mobRealNames;
const char *mobNames[] = {
	"MOBTODD",
	"MOBFEMINIST",
	"MOBCASPAR",
	"MOBMANFOREST1",
	"MOBBANDIT",
	NULL
};

// List of steps to be taken by mobs automatically (including non-auto-mobs).
// The 'int' value is the orient.
map<string, queue<int> > mobQueues;

/**
 * Mobs which automatically walk around aimlessly.
 */
const char *autoMobs[] = {
	"MOBFEMINIST",
	NULL
};

void InitMobs()
{
	autoMobTimer = Timer::Read();

	// Sprites
	mobSprites["MOBTODD"] =			new SpriteSheet("todd.png");
	mobSprites["MOBFEMINIST"] =		new SpriteSheet("feminist.png");
	mobSprites["MOBCASPAR"] =		new SpriteSheet("caspar.png");
	mobSprites["MOBMANFOREST1"] =		new SpriteSheet("man.png");
	mobSprites["MOBBANDIT"] =		new SpriteSheet("bandit.png");

	// Names
	mobRealNames["MOBTODD"] =		"Todd";
	mobRealNames["MOBFEMINIST"] =		"Feminist";
	mobRealNames["MOBCASPAR"] =		"Caspar";
	mobRealNames["MOBMANFOREST1"] =		"Man";
	mobRealNames["MOBBANDIT"] =		"Bandit";

	// Elements
	mobElements["MOBTODD"] =		Element::LIGHT;
	mobElements["MOBFEMINIST"] =		Element::FIRE;
	mobElements["MOBCASPAR"] =		Element::FIRE;
	mobElements["MOBMANFOREST1"] =		Element::AIR;
	mobElements["MOBBANDIT"] =		Element::DARKNESS;
};

void UpdateMobs()
{
	const char **scan = autoMobs;
	if ((Timer::Read()-autoMobTimer) >= 1500)
	{
		autoMobTimer = Timer::Read();
		while (*scan != NULL)
		{
			string name(*scan);
			MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
			if (!state->lock)
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

		if (name == "MOBTODD")
		{
			// Adjust the camera so we can see the player's character.
			int realX = state->x*48+state->offX;
			int realY = state->y*48+state->offY;

			if (realX < (cameraX+MAGIC_BOUND)) cameraX = realX - MAGIC_BOUND;
			if (realX > (cameraX+(SCREEN_WIDTH*48)-MAGIC_BOUND)) cameraX = realX - (SCREEN_WIDTH*48) + MAGIC_BOUND;
			if (realY < (cameraY+MAGIC_BOUND)) cameraY = realY - MAGIC_BOUND;
			if (realY > (cameraY+(SCREEN_HEIGHT*48)-MAGIC_BOUND)) cameraY = realY - (SCREEN_HEIGHT*48) + MAGIC_BOUND;

			if (cameraX < 0) cameraX = 0;
			if (cameraX > (48*(sceneView.width()-SCREEN_WIDTH))) cameraX = 48*(sceneView.width()-SCREEN_WIDTH);
			if (cameraY < 0) cameraY = 0;
			if (cameraY > (48*(sceneView.height()-SCREEN_HEIGHT))) cameraY = (48*(sceneView.height()-SCREEN_HEIGHT));
		};

		scan++;
	};
};

void RenderMobs()
{
	const char **scan = mobNames;
	while (*scan != NULL)
	{
		string name(*scan);
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));

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
	if (name == "MOBFEMINIST")
	{
		MobState *state = (MobState*) GetGameData(name, sizeof(MobState));
		state->lock = 1;
		sceneView.openDialog(dialFeminist, "MOBFEMINIST");
	}
	else if (name == "MOBCASPAR")
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
	else if (name == "MOBBANDIT")
	{
		sceneView.openDialog(dialBanditBoss);
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
