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

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <errno.h>
#endif

using namespace std;

/**
 * This 8-byte header is located at offset 0 in every .sav file.
 */
struct SaveHeader
{
	uint8_t			magic[4];		/* "TODD" */
	uint32_t		count;			/* number of state segments */
} __attribute__ ((packed));

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

void NewGame()
{
	SetPartyMember(0, "CHRTODD");
	Character *charTodd = GetChar("CHRTODD");
	charTodd->setHP(100, 100);
	charTodd->setMP(100, 100);
	charTodd->setXP(0, 100);
	charTodd->setLevel(1);

	SetPartyMember(1, "CHRCASPAR");
	Character *charCaspar = GetChar("CHRCASPAR");
	charCaspar->setHP(120, 120);
	charCaspar->setMP(80, 80);
	charCaspar->setXP(0, 100);
	charCaspar->setLevel(1);

	MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	state->x = 1;
	state->y = 1;
	state->sceneID = Scene::Test;
	state->orient = Mob::DOWN;

	MobState *mobCaspar = (MobState*) GetGameData("MOBCASPAR", sizeof(MobState));
	mobCaspar->sceneID = -1;

	MobState *mobFeminist = (MobState*) GetGameData("MOBFEMINIST", sizeof(MobState));
	mobFeminist->x = 13;
	mobFeminist->y = 4;
	mobFeminist->orient = Mob::LEFT;
	mobFeminist->sceneID = 0;
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

void SaveGame(int slot)
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
