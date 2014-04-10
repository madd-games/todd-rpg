/**
 * Character.cpp
 * Managing characters.
 */

#include "Character.h"
#include <map>
#include "GameState.h"
#include <string.h>
#include "Mob.h"

using namespace std;

map<string, Character*> charMap;

Character::Character(string name, CharInfo *info) : name(name), charInfo(info)
{
	charMap[name] = this;
};

int Character::getHP()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->hp;
};

int Character::getMaxHP()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->maxhp;
};

void Character::setHP(int hp, int maxhp)
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	state->hp = hp;
	if (maxhp != 0) state->maxhp = maxhp;
};

int Character::getMP()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->mp;
};

int Character::getMaxMP()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->maxmp;
};

void Character::setMP(int mp, int maxmp)
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	state->mp = mp;
	if (maxmp != 0) state->maxmp = maxmp;
};

int Character::getXP()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->xp;
};

int Character::getMaxXP()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->maxxp;
};

void Character::setXP(int xp, int maxxp)
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	state->xp = xp;
	if (maxxp != 0) state->maxxp = maxxp;
};

int Character::getLevel()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->level;
};

void Character::setLevel(int level)
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	state->level = level;
};

SpriteSheet *Character::getSpriteSheet()
{
	string mob = charInfo->mob;
	MobInfo info = GetMobInfo(mob);
	return info.mobSprite;
};

string Character::getName()
{
	MobInfo info = GetMobInfo(charInfo->mob);
	return info.name;
};

Character *GetChar(string name)
{
	return charMap[name];
};

string GetPartyMember(int index)
{
	char buffer[17];
	buffer[16] = 0;
	char *party = (char*) GetGameData("PARTY", 64);
	memcpy(buffer, &party[16*index], 16);
	return string(buffer);
};

void SetPartyMember(int index, string name)
{
	char buffer[17];
	strcpy(buffer, name.c_str());
	buffer[16] = 0;
	char *party = (char*) GetGameData("PARTY", 64);
	memcpy(&party[16*index], buffer, 16);
};

// Todd
CharInfo charInfoTodd = {
	"MOBTODD",
	{5, 5, 5, 5, 20, -20, 0, 0},
	{3, 1, 1, 1},
};
Character charTodd("CHRTODD", &charInfoTodd);

// Caspar
CharInfo charInfoCaspar = {
	"MOBCASPAR",
	{20, -30, 3, 3, 0, 0, 0, 0},
	{2, 2, 2, 2},
};
Character charCaspar("CHRCASPAR", &charInfoCaspar);
