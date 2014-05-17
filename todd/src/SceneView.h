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
