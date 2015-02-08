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
 * SkillHealAll.h
 */

int HealAllPartOff[10][2] = {
	{12, 12},	
	{15, 13},
	{14, 16},
	{20, 13},
	{15, 17},
	{18, 19},
	{24, 24},
	{29, 28},
	{23, 24},
	{24, 23}
};

class SkillHealAll : public Skill
{
private:
	int target;
	unsigned int time;
	int stage;

public:
	virtual void init(int target)
	{
		if (target < 4)
		{
			this->target = 0;
		}
		else
		{
			this->target = 4;
		};
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
				int heal = 25 + (3 * battleView.getLevel(battleView.getTurn()));
				int i;
				for (i=0; i<4; i++)
				{
					if (battleView.canMove(target+i))
					{
						battleView.attack(target+i, AttackType::MAGIC, Element::LIGHT, -heal);
					};
				};
			};
			if (stage < 10)
			{
				int i;
				for (i=0; i<4; i++)
				{
					if (battleView.canMove(target+i))
					{
						battleView.emitParticle(target+i, HealAllPartOff[stage][0],
							HealAllPartOff[stage][1], BattleView::SPARK);
					};
				};
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
		return Element::LIGHT;
	};

	virtual string getName()
	{
		return "Heal All";
	};

	virtual string getDesc()
	{
		return "Restores some HP of all allies.";
	};

	virtual int getManaUse()
	{
		return 20;
	};

	virtual bool isMultiTarget()
	{
		return true;
	};

	virtual void configLearning(string &countVar, int &countToLearn, int &itemID)
	{
		countVar = "SKLHEALALL";
		countToLearn = 10;
		itemID = Item::HOLY_SWORD;
	};
};

SkillHealAll skillHealAllVal;
Skill *skillHealAll = &skillHealAllVal;
