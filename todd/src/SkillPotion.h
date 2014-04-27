/**
 * SkillPotion.h
 */

int potionPartOff[10][2] = {
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

class SkillPotion : public Skill
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
				battleView.attack(target, AttackType::DIRECT, Element::NEUTRAL, -100);
				if (battleView.getTurn() < 4)
				{
					Character *chr = GetChar(GetPartyMember(battleView.getTurn()));
					chr->getInventory()->decrItem(Item::POTION);
				};
			};
			if (stage < 10)
			{
				battleView.emitParticle(target, potionPartOff[stage][0], potionPartOff[stage][1], BattleView::SPARK);
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
		return Element::NEUTRAL;
	};

	virtual string getName()
	{
		return "Potion";
	};
};

SkillPotion skillPotionVal;
Skill *skillPotion = &skillPotionVal;
