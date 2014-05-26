/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014, Madd Games.
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

	bool canMove(int entity);
	void schedTurn();
	bool canFlee;
	unsigned long victoryTimer;

	// This vector gets filled with loots at the start of a battle;
	// they get given to the player if they win.
	vector<int> loots;

	// Returns the background image for the specified scene, or NULL.
	SpriteSheet *getBackground(int sceneID);

public:
	void init(Enemy *a, Enemy *b, Enemy *c, Enemy *d);

	virtual void handleEvent(SDL_Event *ev);
	virtual void render();

	CharStats getStats(int entity);
	int *getResistances(int entity);
	int getLevel(int entity);
	int getTurn();
	void getHP(int entity, int &hp, int &maxhp);

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
		FLAME,
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
