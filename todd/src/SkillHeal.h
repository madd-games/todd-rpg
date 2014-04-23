/**
 * SkillHeal.h
 */

class SkillHeal : public Skill
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
		if ((Timer::Read()-time) >= 500)
		{
			stage++;
			if (stage == 1)
			{
				battleView.attack(target, AttackType::MAGIC, Element::LIGHT, -10);
			};
			time = Timer::Read();
		};
	};

	virtual bool isActive()
	{
		return stage < 2;
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
		return "Heal";
	};
};

SkillHeal skillHealVal;
Skill *skillHeal = &skillHealVal;
