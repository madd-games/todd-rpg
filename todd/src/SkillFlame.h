/**
 * SkillFlame.h
 */

class SkillFlame : public Skill
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
				int damage = 50 + 10 * battleView.getLevel(battleView.getTurn());
				battleView.attack(target, AttackType::PHYSICAL, Element::FIRE, damage);
			};
			time = Timer::Read();
		};
	};

	virtual bool isActive()
	{
		return stage < 10;
	};

	virtual bool isOffensive()
	{
		return true;
	};

	virtual int getElement()
	{
		return Element::FIRE;
	};

	virtual string getName()
	{
		return "Flame";
	};

	virtual string getDesc()
	{
		return "Deals physical fire damage by setting the target on fire.";
	};

	virtual int getManaUse()
	{
		return 8;
	};
};

SkillFlame skillFlameVal;
Skill *skillFlame = &skillFlameVal;
