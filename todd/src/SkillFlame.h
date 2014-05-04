/**
 * SkillFlame.h
 */

// TODO: Properly rename to "Fire Slash"

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
		return "Fire Slash";
	};

	virtual string getDesc()
	{
		return "Deals physical fire damage by slashing the target with a burning sword.";
	};

	virtual int getManaUse()
	{
		return 8;
	};
};

SkillFlame skillFlameVal;
Skill *skillFlame = &skillFlameVal;
