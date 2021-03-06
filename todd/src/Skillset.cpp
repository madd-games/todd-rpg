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
 * Skillset.cpp
 * Defines skillsets.
 */

#include "Skill.h"
#include "Skillset.h"
#include <map>

using namespace std;

map<string, Skillset*> skillsets;

Skillset skillsetTodd;
Skillset skillsetCaspar;
Skillset skillsetBob;

void InitSkillsets()
{
	// Todd
	skillsets["CHRTODD"] = &skillsetTodd;
	skillsetTodd.name = "Healing Magic";
	skillsetTodd.skills.clear();
	skillsetTodd.skills.push_back(skillHeal);
	skillsetTodd.skills.push_back(skillHealAll);
	skillsetTodd.skills.push_back(skillShield);

	// Caspar
	skillsets["CHRCASPAR"] = &skillsetCaspar;
	skillsetCaspar.name = "Fire Magic";
	skillsetCaspar.skills.clear();
	skillsetCaspar.skills.push_back(skillFireSlash);

	// Bob
	skillsets["CHRBOB"] = &skillsetBob;
	skillsetBob.name = "Elements";
	skillsetBob.skills.clear();
	skillsetBob.skills.push_back(skillSplash);
	skillsetBob.skills.push_back(skillBurn);
};

Skillset *GetSkillset(string charName)
{
	return skillsets[charName];
};
