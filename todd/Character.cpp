/**
 * Character.cpp
 * Managing characters.
 */

#include "Character.h"
#include <map>
#include "GameState.h"
#include <string.h>

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
	{3, 1, 1, 1}
};
Character charTodd("CHRTODD", &charInfoTodd);
