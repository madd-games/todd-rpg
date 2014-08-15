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
 * Mob.h
 * A header file for handling mobs.
 */

#ifndef MOB_H
#define MOB_H

#include "SpriteSheet.h"
#include "Element.h"

/**
 * \brief Stores the state of a mob.
 */
struct MobState
{
	/**
	 * \brief X position of the mob in the scene (in 48x48 units).
	 */
	int x;

	/**
	 * \brief Y position of the mob in the scene (in 48x48 units)
	 */
	int y;

	/**
	 * \brief Orientation of the mob.
	 * \sa Mob::Orientation
	 */
	int orient;

	/**
	 * \brief Which scene the mob is currently located in.
	 */
	int sceneID;

	/**
	 * \brief Velocity along the X axis.
	 */
	int motX;

	/**
	 * \brief Velocity across the Y axis.
	 */
	int motY;

	/**
	 * \brief Offset (in pixels) from the grid position.
	 */
	int offX;

	/**
	 * \brief Offset (in pixels) from the grid position.
	 */
	int offY;
	int steps;

	// Timer
	unsigned long stepTimer;

	/**
	 * If this is nonzero, the mob is locked (doesn't walk around).
	 * Applies to auto-mobs only.
	 */
	int lock;

	/**
	 * \brief Tells the mob to begin moving, this includes animation.
	 * \param orient The direction to move in.
	 * \param force If true, the mob will walk even into walls If false, and there is a wall in the way,
	 *              the mob will only turn around.
	 */
	void beginMove(int orient, bool force = false);
};

struct MobInfo
{
	/**
	 * The sprite sheet for the mob.
	 */
	SpriteSheet *mobSprite;

	/**
	 * The actual name for the mob (as shown to the player)
	 */
	string name;

	/**
	 * The mob's element.
	 */
	int element;
};

void InitMobs();

/**
 * \brief Update all mobs.
 */
void UpdateMobs();

/**
 * \brief Render all mobs in the current scene.
 */
void RenderMobs();

/**
 * \brief Returns the name of the mob at the specified location, empty string if there
 * isn't one.
 */
string GetMobAt(int scene, int x, int y);

/**
 * \brief Returns true if there is a mob at the specified position.
 */
bool IsMobAt(int scene, int x, int y);

/**
 * \brief Called upon interaction with a mob. 
 */
void InteractWithMob(string name);

/**
 * \brief Get mob info.
 */
MobInfo GetMobInfo(string name);

/**
 * \brief Returns true if the specified mob is not doing anything (the command queue is empty).
 */
bool IsMobReady(string name);

/**
 * \brief Enqueues a "move" action for a mob.
 * \param name The internal name for the mob, e.g. MOBCASPAR.
 * \param dir Which direction to move, e.g. Mob::LEFT.
 * \param count How many tiles to move.
 */
void EnqueueMobMoves(string name, int dir, int count);

/**
 * \brief Mob-related stuff.
 */
namespace Mob
{
	/**
	 * \brief Mob orientations.
	 */
	enum Orientation
	{
		DOWN = 0,
		LEFT = 1,
		RIGHT = 2,
		UP = 3,
	};
};

#endif
