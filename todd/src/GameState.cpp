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

	//SetPartyMember(1, "CHRCASPAR");
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

	// Chests
//	InitChest(0, 8, 1, Item::POTION, 10);
//	InitChest(0, 9, 1, Item::WOODEN_SHIELD, 1);
	InitChest(Scene::Forest, 18, 2, Item::POTION, 1);

	// Doors
	InitDoor(Scene::Castle_ToddRoom, 13, 0, Scene::Castle, 18, 7, Mob::DOWN);
	InitDoor(Scene::Castle, 18, 6, Scene::Castle_ToddRoom, 13, 1, Mob::DOWN);
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
