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
		Test = 0,
	};

	static void LoadScenes();
	static Scene *GetSceneByID(int id);

private:
	struct Tile
	{
		int id;
		int meta;
	};

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

public:
	Scene(string name);
	~Scene();

	friend class SceneView;
};

#endif
