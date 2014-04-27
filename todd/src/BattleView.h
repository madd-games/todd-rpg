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
		DIRECT,
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
			ITEM_SELECT,
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

	struct Particle
	{
		SpriteSheet *ss;
		unsigned long timeStart;
		unsigned long timeStage;		// time taken between each stage
		int numStages;
		int x, y;				// of the top-left corner.
	};

	vector<DamageDisplay> dmgDisplays;
	vector<Particle> particles;

	vector<Enemy*> enemies;
	int turn;
	int mode;

	int optionSel;
	int targetSel;
	int skillSelIndex;
	int itemSelIndex;

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

	// Particle types.
	enum
	{
		SPARK,
	};

	/**
	 * \brief Emit a new particle.
	 *
	 * The coordinates offX and offY specify the position of the CENTER of the particle, relative
	 * to the entity.
	 *
	 * \param entity The entity on which this particle shall be emitted.
	 * \param offX Offset from the left side of the entity where the particle shall appear.
	 * \param offY Offset from the top of the entity where the particle shall appear.
	 * \param type Type of particle (see above enum).
	 */
	void emitParticle(int entity, int offX, int offY, int type);
};

extern BattleView battleView;

void StartBattle(Enemy *a, Enemy *b = NULL, Enemy *c = NULL, Enemy *d = NULL);

#endif
