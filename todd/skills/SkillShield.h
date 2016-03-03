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
 * SkillShield.h
 */

class SkillShield : public Skill
{
private:
	int target;
	unsigned int time;
	int stage;

public:
	virtual void init(int target)
	{
		this->target = target;
		time = Timer::Read();
		stage = 0;
	};

	virtual void act()
	{
		if ((Timer::Read()-time) >= 50)
		{
			stage++;
			if (stage == 2)
			{
				battleView.inflictStatus(target, StatusEffect::SHIELD);
			};
			time = Timer::Read();
		};
	};

	virtual bool isActive()
	{
		return stage < 20;
	};

	virtual bool isOffensive()
	{
		return false;
	};

	virtual int getElement()
	{
		return Element::NEUTRAL;
	};

	virtual string getName()
	{
		return "Shield";
	};

	virtual string getDesc()
	{
		return "Inflicts the SHIELD status effect, causing 50% resistance to all physical attacks.";
	};

	virtual int getManaUse()
	{
		return 20;
	};

	virtual void configLearning(string &countVar, int &countToLearn, int &itemID)
	{
		countVar = "SKLSHIELD";
		countToLearn = 12;
		itemID = Item::SHADOW_SHIELD;
	};
};

SkillShield skillShieldVal;
Skill *skillShield = &skillShieldVal;
