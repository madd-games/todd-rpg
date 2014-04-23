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

class SceneView : public View
{
private:
	Scene *scene;
	void renderLayer(Scene::Tile *layer);
	int sceneID;

	bool keyLeft, keyRight, keyUp, keyDown;
	GUI *gui;

public:
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
};

extern SceneView sceneView;

#endif
