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

void InitSkillsets()
{
	// Todd
	skillsets["CHRTODD"] = &skillsetTodd;
	skillsetTodd.name = "Healing Magic";
	skillsetTodd.skills.clear();
	skillsetTodd.skills.push_back(skillHeal);

	// Caspar
	skillsets["CHRCASPAR"] = &skillsetCaspar;
	skillsetCaspar.name = "Fire Magic";
	skillsetCaspar.skills.clear();
	skillsetCaspar.skills.push_back(skillFlame);
};

Skillset *GetSkillset(string charName)
{
	return skillsets[charName];
};
