/**
 * BattleView.h
 * Displays battles.
 */

#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include "View.h"
#include "Enemy.h"
#include "Skill.h"
#include <vector>
#include "CharStats.h"
#include "Element.h"

using namespace std;

namespace AttackType
{
	enum
	{
		PHYSICAL,
		MAGIC,
	};
};

class BattleView : public View
{
private:
	class Mode
	{
	public:
		enum
		{
			MENU,
			TARGET,
			SKILL,
			ITEMS,
			VICTORY,
			SKILL_SELECT,
		};
	};

	struct DamageDisplay
	{
		int element;
		string value;
		int red, green, blue;
		int x, y;
		unsigned long start;
	};

	vector<DamageDisplay> dmgDisplays;

	vector<Enemy*> enemies;
	int turn;
	int mode;

	int optionSel;
	int targetSel;
	int skillSelIndex;

	Skill *skillSel;

	bool canMove(int entity);
	void schedTurn();
	bool canFlee;
	unsigned long victoryTimer;

	// This vector gets filled with loots at the start of a battle;
	// they get given to the player if they win.
	vector<int> loots;

public:
	void init(Enemy *a, Enemy *b, Enemy *c, Enemy *d);

	virtual void handleEvent(SDL_Event *ev);
	virtual void render();

	CharStats getStats(int entity);
	int *getResistances(int entity);
	int getLevel(int entity);
	int getTurn();

	/**
	 * \brief Inflict damage on a target.
	 *
	 * \param target The target getting hit.
	 * \param type AttackType::PHYSICAL or AttackType::MAGIC.
	 * \param element The element.
	 * \param damage The normal damage (not accounting for STR, DEF, etc.)
	 */
	void attack(int target, int type, int element, int damage);

	int getRandomAlly(bool allowDead = false);
	void setFlee(bool flee);
};

extern BattleView battleView;

void StartBattle(Enemy *a, Enemy *b = NULL, Enemy *c = NULL, Enemy *d = NULL);

#endif
