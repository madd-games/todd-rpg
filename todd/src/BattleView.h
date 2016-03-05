/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014-2016, Madd Games.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer. 
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===============================================================================================
*/

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
#include "StatusEffect.h"

using namespace std;

/**
 * \brief Types of attacks.
 */
namespace AttackType
{
	enum
	{
		/**
		 * \brief Physical attacks, affected by STR and DEF.
		 */
		PHYSICAL,

		/**
		 * \brief Magic attacks, affected by INT and MDEF.
		 */
		MAGIC,

		/**
		 * \brief Direct attacks, deal damage without being affected by any stats.
		 *
		 * This is used, for example, by items.
		 */
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
			GAME_OVER,
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
		float vx, vy;
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

	void schedTurn();
	void cleanup();
	bool canFlee;
	unsigned long victoryTimer;

	// This vector gets filled with loots at the start of a battle;
	// they get given to the player if they win.
	vector<int> loots;

	// Returns the background image for the specified scene, or NULL.
	SpriteSheet *getBackground(int sceneID);

	// Returns the status effect set for the specified entity.
	StatusEffectSet getStatusEffectSet(int entity);

public:
	bool canMove(int entity);

	/**
	 * \brief Set up a battle with the specified enemies.
	 *
	 * Once the battle ends, the enemies will be deleted!
	 */
	void init(Enemy *a, Enemy *b, Enemy *c, Enemy *d);

	virtual void handleEvent(SDL_Event *ev);
	virtual void render();

	/**
	 * \brief Get the stats for an entity.
	 * \param entity The entity to probe (0-3 = allies, 4-7 = enemies).
	 * \return The CharStats structure for the entity.
	 */
	CharStats getStats(int entity);

	/**
	 * \brief Get the resistances for elements for an entity.
	 * \param entity The entity to probe (0-3 = allies, 4-7 = enemies).
	 * \return An array whose indices are element ID (see Element), and the values specify
	 *         the resistance in %.
	 */
	int *getResistances(int entity);

	/**
	 * \brief Get the level of an entity.
	 * \param entity The entity to probe (0-3 = allies, 4-7 = enemies).
	 * \return The entity's level.
	 */
	int getLevel(int entity);

	/**
	 * \brief Check whose turn it is.
	 * \return The entity whose turn it is (0-3 = allies, 4-7 = enemies).
	 */
	int getTurn();

	/**
	 * \brief Get an enemy's HP and max HP.
	 * \param entity The entity to probe (0-3 = allies, 4-7 = enemies).
	 * \param hp Reference to an int variable where HP shall be stored.
	 * \param maxhp Reference to an int varbiable where max HP shall be stored.
	 */
	void getHP(int entity, int &hp, int &maxhp);

	/**
	 * \brief Inflict damage on a target.
	 *
	 * \param target The target getting hit (0-3 = allies, 4-7 = enemies).
	 * \param type AttackType::PHYSICAL or AttackType::MAGIC.
	 * \param element The element.
	 * \param damage The normal damage (not accounting for STR, DEF, etc.)
	 *
	 * \return The actual damage deal, with all factors taken into account.
	 */
	int attack(int target, int type, int element, int damage);

	/**
	 * \brief Restore the target's mana.
	 * \param target The target whose mana is to be restored (0-3 = allies, 4-7 = enemies).
	 * \param mp The amount of mana to restore.
	 */
	void restoreMana(int target, int mp);

	/**
	 * \brief Inflict a status effect on a target.
	 * \param target The target (0-3 = allies, 4-7 = enemies).
	 * \param effect The status effect, from the enum in StatusEffect.
	 */
	void inflictStatus(int target, int effect);

	/**
	 * \brief Remove a status effect from a target.
	 * \param target The target (0-3 = allies, 4-7 = enemies).
	 * \param effect The status effect, from the enum in StatusEffect.
	 */
	void removeStatus(int target, int effect);

	/**
	 * \brief Check if an entity has the specified status effect.
	 * \param entity The entity (0-3 = allies, 4-7 = enemies).
	 * \param effect The status effect to probe, from the enum in StatusEffect.
	 * \return true if this entity has the specified status effect.
	 */
	bool hasStatusEffect(int entity, int effect);

	/**
	 * \brief Get a random ally.
	 * \param allowDead If true, dead allies may be returned (default = false).
	 * \return The ally entity number (0-3), the same as the party index.
	 */
	int getRandomAlly(bool allowDead = false);

	/**
	 * \brief Change whether it is possible to flee from this battle.
	 * \param flee If true, the player may choose to flee.
	 */
	void setFlee(bool flee);

	/**
	 * \brief Particle types.
	 */
	enum
	{
		SPARK,
		FLAME,
		SPLASH,
		POISON_BUBBLE,
		BLOOD_EXPLODE,
		BLOOD_IMPLODE,
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
