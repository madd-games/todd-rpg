/**
 * Mob.h
 * A class for handling mobs.
 */

#ifndef MOB_H
#define MOB_H

#include "SpriteSheet.h"
#include "Element.h"

struct MobState
{
	// Position of the mob in the scene (in the 48x48 units)
	int x, y;

	// Orientation (see below)
	int orient;

	// Which scene this mob resides on.
	int sceneID;

	// Motion info
	int motX, motY;
	int offX, offY;
	int steps;

	// Timer
	unsigned long stepTimer;

	// If this is nonzero, the mob is locked (doesn't walk around)
	int lock;

	void beginMove(int orient);
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

class Mob
{
public:
	// Mob orientations
	enum
	{
		DOWN = 0,
		LEFT = 1,
		RIGHT = 2,
		UP = 3,
	};
};

#endif