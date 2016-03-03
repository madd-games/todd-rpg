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
 * SceneView.h
 * The "scene" view, where the player can walk around a scene.
 */

#ifndef SCENE_VIEW_H
#define SCENE_VIEW_H

#include "View.h"
#include "Scene.h"
#include "GUI.h"
#include "Dialog.h"
#include "RandomBattle.h"

#ifdef TODD_DEBUG
#include "DebugOptionsView.h"
#endif

class SceneView : public View
{
private:
	Scene *scene;
	void renderLayer(Scene::Tile *layer);
	int sceneID;

	bool keyLeft, keyRight, keyUp, keyDown;
	GUI *gui;

	int stepCount;
	unsigned long battleTimer;
	RandomBattle randomBattle;

public:
	// Debugging options. Those can be enabled and disabled through a menu
	// that can be opened with F6 in the debug build, but is not available
	// in the standard release.
	bool ghostWalk;
	bool enableRandomBattles;

	SceneView();

	virtual void handleEvent(SDL_Event *ev);
	virtual void render();
	void setScene(int id);
	int getScene();
	int width();
	int height();

	/**
	 * \brief Returns true if it's possible to walk onto the specified tile.
	 */
	bool canWalk(int x, int y);

	/**
	 * \brief Opens a conversation.
	 */
	void openDialog(DialogEntry *entry, string with = "");

	/**
	 * \brief Opens a GUI.
	 */
	void openGUI(GUI *gui);

	friend class LootView;
#ifdef TODD_DEBUG
	friend class DebugOptionsView;
#endif
};

extern SceneView sceneView;

#endif
