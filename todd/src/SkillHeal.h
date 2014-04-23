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
				CharStats stats = battleView.getStats(battleView.getTurn());
				int heal = 2 * (stats.INT + battleView.getLevel(battleView.getTurn())/2);
				battleView.attack(target, AttackType::MAGIC, Element::LIGHT, -heal);
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

	virtual string getDesc()
	{
		return "Restores the target's HP using LIGHT-elemental healing.";
	};
};

SkillHeal skillHealVal;
Skill *skillHeal = &skillHealVal;
