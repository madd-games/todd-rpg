/**
 * SkillAttack.h
 */

class SkillAttack : public Skill
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
				battleView.attack(target, AttackType::PHYSICAL, Element::NEUTRAL, 10);
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
		return true;
	};
};

SkillAttack skillAttackVal;
Skill *skillAttack = &skillAttackVal;
