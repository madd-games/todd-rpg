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
 * Scene.h
 * Manages a scene.
 */

#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "Struct.h"

using namespace std;

class Scene
{
public:
	/**
	 * Scene IDs.
	 * Make sure you explicitily assign a number to each one so that they don't mess up
	 * if we have to remove one or reorder or something.
	 */
	enum
	{
		Forest = 1,
		Castle_ToddRoom = 2,
		Castle = 3,
		Eastville = 4,
		Eastville_House1 = 5,
	};

	static void LoadScenes();
	static Scene *GetSceneByID(int id);

private:
	struct Tile
	{
		int id;
		int meta;
	};

	// Describes a warp point.
	struct WarpPoint
	{
		int x;
		int y;
		int targetScene;		// negative is overworld map spot indices.
	};

	// Describes a placed structure.
	struct StructInfo
	{
		Struct *str;
		int x;
		int y;
	};

	// All the warp points in this scene.
	vector<WarpPoint> warpPoints;

	// All the structures in the scene.
	vector<StructInfo> structList;

	// Width and height of the board.
	int width, height;

	// Offset of the board.
	int offX, offY;

	// Background layer - this is rendered before anything else.
	// The player walks on this.
	Tile *bgLayer;

	// The 'hard' layer - the player can't walk on this at all.
	// Rendered after the background layer.
	Tile *hardLayer;

	// The overlay layer - the player can walk on this, but it's rendered
	// on top of all mobs - this includes stuff like tall grass.
	Tile *ovLayer;

	// Processing layers.
	void processTile(vector<string> &lines, int x, int y, Tile *layer, map<char, Tile> &tileMap);

	// Called when the player moves with the specified relative vector.
	// If there is a warp at the target position, then the function returns true,
	// and the player is moved to that position.
	// Otherwise, the function returns false and no motion occurs.
	bool checkWarp(int relX, int relY);

public:
	Scene(string name);
	~Scene();

	friend class SceneView;
};

#endif
