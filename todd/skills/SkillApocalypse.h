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
 * SkillApocalypse.h
 */

class SkillApocalypse : public Skill
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
		if ((Timer::Read()-time) >= 10)
		{
			stage++;
			if (stage == 2)
			{
				int i, damage;
				for (i=target; i<(target+4); i++)
				{
					if (battleView.canMove(i))
					{
						damage = RandomUniform(80, 100) + RandomUniform(4, 6) * battleView.getLevel(battleView.getTurn());
						battleView.attack(i, AttackType::MAGIC, Element::DARKNESS, damage);
					};
				};
			};
			time = Timer::Read();
		};
	};

	virtual bool isActive()
	{
		return stage < 100;
	};

	virtual bool isOffensive()
	{
		return true;
	};

	virtual int getElement()
	{
		return Element::DARKNESS;
	};

	virtual string getName()
	{
		return "Apocalypse";
	};

	virtual string getDesc()
	{
		return "Strikes all enemies, dealing powerful DARKNESS damage.";
	};

	virtual int getManaUse()
	{
		return 20;
	};
};

SkillApocalypse skillApocalypseVal;
Skill *skillApocalypse = &skillApocalypseVal;
