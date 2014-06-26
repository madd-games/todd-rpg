/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014, Madd Games.
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
 * GameState.cpp
 * Managing the game state.
 */

#include "GameState.h"
#include <map>
#include <string.h>
#include "Mob.h"
#include <sstream>
#include <inttypes.h>
#include <fstream>
#include "Todd.h"
#include "Scene.h"
#include "Character.h"
#include "Item.h"
#include "ActiveTile.h"
#include "SceneView.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <errno.h>
#endif

using namespace std;

/**
 * This 20-byte header is prefixed before every segment in a .sav file.
 */
struct SegmentHeader
{
	uint32_t		size;
	uint8_t			name[16];
} __attribute__ ((packed));

struct GameStateSegment
{
	void *ptr;
	size_t size;
};

map<string, GameStateSegment> gameState;

void InitChest(int scene, int x, int y, int id, int amount = 1)
{
	ChestState *state = (ChestState*) GetTileState(scene, x, y, GetActiveTile(4));
	state->id = id;
	state->amount = amount;
};

void InitDoor(int scene, int x, int y, int targetScene, int targetX, int targetY, int orient)
{
	DoorState *state = (DoorState*) GetTileState(scene, x, y, GetActiveTile(8));
	state->targetScene = targetScene;
	state->x = targetX;
	state->y = targetY;
	state->orient = orient;
};

void NewGame()
{
	gameState.clear();

	SetPartyMember(0, "CHRTODD");
	Character *charTodd = GetChar("CHRTODD");
	charTodd->setHP(100, 100);
	charTodd->setMP(100, 100);
	charTodd->setXP(0, 100);
	charTodd->setLevel(1);
	ItemStack stackBasicSword = {2, 1};
	charTodd->getInventory()->set(0, stackBasicSword);

	Character *charCaspar = GetChar("CHRCASPAR");
	charCaspar->setHP(120, 120);
	charCaspar->setMP(80, 80);
	charCaspar->setXP(0, 100);
	charCaspar->setLevel(1);
	charCaspar->getInventory()->set(0, stackBasicSword);

	MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	state->x = 10;
	state->y = 3;
	state->sceneID = Scene::Castle_ToddRoom;
	sceneView.setScene(state->sceneID);
	state->orient = Mob::DOWN;

	MobState *mobCaspar = (MobState*) GetGameData("MOBCASPAR", sizeof(MobState));
	mobCaspar->sceneID = Scene::Castle;
	mobCaspar->x = 10;
	mobCaspar->y = 5;
	mobCaspar->orient = Mob::DOWN;

	MobState *mobFeminist = (MobState*) GetGameData("MOBFEMINIST", sizeof(MobState));
	mobFeminist->x = 13;
	mobFeminist->y = 4;
	mobFeminist->orient = Mob::LEFT;
	mobFeminist->sceneID = 0;

	// The guy that stands around in the forest and blocks the path until you
	// get Caspar.
	state = (MobState*) GetGameData("MOBMANFOREST1", sizeof(MobState));
	state->x = 3;
	state->y = 4;
	state->orient = Mob::LEFT;
	state->sceneID = Scene::Forest;

	// The guy that blocks the path in the forest.
	state = (MobState*) GetGameData("MOBMANFOREST2", sizeof(MobState));
	state->x = 11;
	state->y = 10;
	state->orient = Mob::RIGHT;
	state->sceneID = Scene::Forest;

	// Bandit boss
	state = (MobState*) GetGameData("MOBBANDIT", sizeof(MobState));
	state->x = 26;
	state->y = 4;
	state->orient = Mob::LEFT;
	state->sceneID = Scene::Forest;

	// One of the villagers in Eastville.
	state = (MobState*) GetGameData("MOBMANEASTV1", sizeof(MobState));
	state->x = 12;
	state->y = 3;
	state->orient = Mob::DOWN;
	state->sceneID = Scene::Eastville_House1;

	// Chests
	InitChest(Scene::Forest, 18, 2, Item::POTION, 5);
	InitChest(Scene::Forest, 19, 4, Item::WOODEN_SHIELD, 1);
	InitChest(Scene::Eastville_House1, 23, 1, Item::MANA_FRUIT, 5);
	InitChest(Scene::Eastville_House1, 24, 1, Item::CHAIN_ARMOR, 1);

	// Doors
	InitDoor(Scene::Castle_ToddRoom, 13, 0, Scene::Castle, 18, 7, Mob::DOWN);
	InitDoor(Scene::Castle, 18, 6, Scene::Castle_ToddRoom, 13, 1, Mob::DOWN);
	InitDoor(Scene::Eastville, 8, 9, Scene::Eastville_House1, 11, 6, Mob::UP);
	InitDoor(Scene::Eastville, 16, 9, Scene::Eastville, 10, 1, Mob::UP);
	InitDoor(Scene::Eastville_House1, 11, 7, Scene::Eastville, 8, 10, Mob::DOWN);
};

string GetPathToSaveSlot(int slot)
{
#ifdef _WIN32
	char *env = getenv("APPDATA");
	string path = env;
	path += "\\madd.todd-saves";

	BOOL create = CreateDirectory(path.c_str(), NULL);
	if (!create)
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			GameAbort(string("CreateDirectory() failed: ") + path);
		};
	};

	stringstream ss;
	ss << path << "\\" << slot << ".sav";
#else
	char *home = getenv("HOME");
	string path = home;
	path += "/madd.todd-saves";

	int create = mkdir(path.c_str(), 0755);
	if (create != 0)
	{
		if (errno != EEXIST)
		{
			GameAbort(string("mkdir() failed: ") + path + ": " + strerror(errno));
		};
	};

	stringstream ss;
	ss << path << "/" << slot << ".sav";
#endif

	return ss.str();
};

void LoadGame(int slot)
{
	string path = GetPathToSaveSlot(slot);
	ifstream ifs(path.c_str(), ios::binary);
	if (ifs.fail())
	{
		return;
	};

	SaveHeader savhead;
	ifs.read((char*) &savhead, sizeof(SaveHeader));

	if (memcmp(savhead.magic, "TODD", 4) != 0)
	{
		GameAbort(string("The save file ") + path + " is invalid: invalid signature");
	};

	size_t count = savhead.count;
	while (count--)
	{
		SegmentHeader seghead;
		ifs.read((char*) &seghead, sizeof(SegmentHeader));
		char buffer[17];
		memcpy(buffer, seghead.name, 16);
		buffer[16] = 0;
		string name = buffer;

		void *state = GetGameData(name, seghead.size);
		ifs.read((char*) state, seghead.size);
	};

	ifs.close();
};

void SaveGame(int slot, string name)
{
	string path = GetPathToSaveSlot(slot);
	ofstream ofs(path.c_str(), ios::binary);
	if (ofs.fail())
	{
		GameAbort(string("Cannot open ") + path + " in write mode.");
	};

	SaveHeader savhead;
	memcpy(savhead.magic, "TODD", 4);
	savhead.count = gameState.size();
	char buf[33];
	strcpy(buf, name.c_str());
	memcpy(savhead.name, buf, 32);
	ofs.write((const char*) &savhead, sizeof(SaveHeader));

	map<string, GameStateSegment>::iterator it;
	for (it=gameState.begin(); it!=gameState.end(); ++it)
	{
		SegmentHeader seghead;
		seghead.size = it->second.size;
		char buffer[17];
		strcpy(buffer, it->first.c_str());
		memcpy(seghead.name, buffer, 16);
		ofs.write((const char*) &seghead, sizeof(SegmentHeader));
		ofs.write((const char*) it->second.ptr, seghead.size);
	};

	ofs.close();
};

void *GetGameData(string name, size_t size)
{
	if (gameState.count(name) == 0)
	{
		GameStateSegment seg;
		seg.ptr = malloc(size);
		memset(seg.ptr, 0, size);
		seg.size = size;
		gameState[name] = seg;
		return seg.ptr;
	}
	else
	{
		GameStateSegment seg = gameState[name];
		if (seg.size < size)
		{
			void *newptr = malloc(size);
			memset(newptr, 0, size);
			memcpy(newptr, seg.ptr, seg.size);
			free(seg.ptr);
			seg.ptr = newptr;
			seg.size = size;
			gameState[name] = seg;
		};
		return seg.ptr;
	};
};
