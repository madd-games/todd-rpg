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
				int element;
				if (battleView.getTurn() < 4)
				{
					Character *chr = GetChar(GetPartyMember(battleView.getTurn()));
					Item *wep = GetItem(chr->getInventory()->get(0).id);
					element = wep->getElement();
				}
				else
				{
					element = Element::NEUTRAL;
				};

				CharStats stats = battleView.getStats(battleView.getTurn());
				int damage = 2 * (stats.STR + battleView.getLevel(battleView.getTurn()));
				battleView.attack(target, AttackType::PHYSICAL, element, damage);
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

	virtual int getElement()
	{
		return Element::NEUTRAL;
	};

	virtual string getName()
	{
		return "Attack";
	};
};

SkillAttack skillAttackVal;
Skill *skillAttack = &skillAttackVal;
