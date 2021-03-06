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

Character::Character(string name, CharInfo *info, string invName) : name(name), charInfo(info), inv(invName, 46)
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

void Character::train(int xp)
{
	setXP(getXP()+xp);
	while (getXP() >= getMaxXP())
	{
		setXP(getXP()-getMaxXP(), getMaxXP() * 1.5);
		setLevel(getLevel()+1);
		setHP(getHP(), getMaxHP()*1.3);
		setMP(getMP(), getMaxMP()*1.2);
		
		CharState *state = (CharState*)GetGameData(name, sizeof(CharState));
		state->stats.STR += charInfo->levelUpStats.STR;
		state->stats.INT += charInfo->levelUpStats.INT;
		state->stats.DEF += charInfo->levelUpStats.DEF;
		state->stats.MDEF += charInfo->levelUpStats.MDEF;
		state->stats.ACC += charInfo->levelUpStats.ACC;
		state->stats.AGI += charInfo->levelUpStats.AGI;
	};
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

void Character::trainWeapon()
{
	// find out what weapon type te player is using
	Container *inv = getInventory();
	ItemStack weaponStack = inv->get(0);
	Item *wep = GetItem(weaponStack.id);
	int wepType = wep->getWeaponType();
	
	// Increasing number of weapon uses
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	int &count = state->numTypeUses[wepType];
	if (count < 500)
	{
		count++;
	};
};

int Character::getWeaponLevel()
{
	// find out what weapon type the player is using
	Container *inv = getInventory();
	ItemStack weaponStack = inv->get(0);
	Item *wep = GetItem(weaponStack.id);
	int wepType = wep->getWeaponType();

	CharState *state = (CharState*)GetGameData(name, sizeof(CharState));
	return state->numTypeUses[wepType] / 50;
};

int Character::getElement()
{
	MobInfo info = GetMobInfo(charInfo->mob);
	return info.element;
};
void Character::init()
{
	CharState *state = (CharState*)GetGameData(name, sizeof(CharState));
	memcpy(&state->stats, &charInfo->stats, sizeof(CharStats));
};

CharStats *Character::getStats()
{
	CharState *state = (CharState*)GetGameData(name, sizeof(CharState));
	return &state->stats;
};

int *Character::getResistances()
{
	return charInfo->resist;
};

StatusEffectSet &Character::getStatusEffectSet()
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	return state->ses;
};

void Character::dealDirectDamage(int dmg)
{
	CharState *state = (CharState*) GetGameData(name, sizeof(CharState));
	state->hp -= dmg;
	if (state->hp <= 0)
	{
		getStatusEffectSet().clear();
		state->hp = 0;
	};
	if (state->hp > state->maxhp) state->hp = state->maxhp;
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

Container *Character::getInventory()
{
	return &inv;
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
	{3, 1, 1, 1, 0, 0},
	{2, 5, 0, 2, 60, 5},
};
Character charTodd("CHRTODD", &charInfoTodd, "INVTODD");

// Caspar
CharInfo charInfoCaspar = {
	"MOBCASPAR",
	{20, -30, 3, 3, 0, 0, 0, 0},
	{2, 2, 2, 2, 0, 0},
	{5, 2, 2, 0, 80, 10},
};
Character charCaspar("CHRCASPAR", &charInfoCaspar, "INVCASPAR");

// Bob
CharInfo charInfoBob = {
	"MOBBOB",
	{90, 70, 0, -50, 0, 0, 0, 0},
	{2, 2, 2, 2, 0, 0},
	{1, 7, -1, 10, 60, 10},
};
Character charBob("CHRBOB", &charInfoBob, "INVBOB");
