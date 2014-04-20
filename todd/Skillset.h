/**
 * Skillset.h
 * A class representing a character's skill set.
 */

#ifndef SKILLSET_H
#define SKILLSET_H

#include <string>
#include <vector>
#include "Skill.h"

using namespace std;

class Skillset
{
public:
	string name;
	vector<Skill*> skills;
};

extern Skillset skillsetTodd;
extern Skillset skillsetCaspar;

void InitSkillsets();
Skillset *GetSkillset(string charName);

#endif
