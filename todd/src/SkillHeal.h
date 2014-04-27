/**
 * SkillHeal.h
 */

int healPartOff[10][2] = {
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
		if ((Timer::Read()-time) >= 50)
		{
			stage++;
			if (stage == 2)
			{
				CharStats stats = battleView.getStats(battleView.getTurn());
				int heal = 2 * (stats.INT + battleView.getLevel(battleView.getTurn())/2);
				battleView.attack(target, AttackType::MAGIC, Element::LIGHT, -heal);
			};
			if (stage < 10)
			{
				battleView.emitParticle(target, healPartOff[stage][0], healPartOff[stage][1], BattleView::SPARK);
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

	virtual int getManaUse()
	{
		return 6;
	};
};

SkillHeal skillHealVal;
Skill *skillHeal = &skillHealVal;
