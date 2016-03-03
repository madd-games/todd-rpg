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
 * SkillSplash.h
 */

class SkillSplash : public Skill
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
		if ((Timer::Read()-time) >= 10)
		{
			stage++;
			if (stage == 2)
			{
				int count = 50;
				while (count--) battleView.emitParticle(target, 0, 0, BattleView::SPLASH);
				int damage = RandomUniform(40, 60) + RandomUniform(5, 10) * battleView.getLevel(battleView.getTurn());
				battleView.attack(target, AttackType::MAGIC, Element::WATER, damage);
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
		return Element::WATER;
	};

	virtual string getName()
	{
		return "Splash";
	};

	virtual string getDesc()
	{
		return "Deals water damage to the target.";
	};

	virtual int getManaUse()
	{
		return 10;
	};
};

SkillSplash skillSplashVal;
Skill *skillSplash = &skillSplashVal;
