/**
 * OverworldView.cpp
 * The "overworld map" view.
 */

#include "OverworldView.h"
#include "SpriteSheet.h"
#include "Scene.h"
#include "Mob.h"
#include "GameState.h"
#include "SceneView.h"
#include "Todd.h"
#include "Text.h"
#include "Options.h"

OverworldView overworldView;

struct MapSpot
{
	// An ID assigned to this map spot.
	int id;

	// Position of the center of the spot on the map.
	int x, y;

	// Which spots to go to when we press left/right/up/down
	// (0 means you can't move in that direction).
	int left, right, up, down;

	// Scene, coordinates, and orientation.
	int sceneID;
	int sceneX, sceneY;
	int sceneOrient;

	// The name of the area.
	const char *name;
};

vector<MapSpot> mapSpots;

MapSpot mapSpot0 = {1, 283, 112, 0, 0, 0, 2, Scene::Castle, 10, 14, Mob::UP, "Castle"};
MapSpot mapSpot1 = {2, 367, 280, 0, 0, 1, 0, Scene::Forest, 1, 2, Mob::DOWN, "Forest (West)"};
MapSpot mapSpot2 = {3, 407, 280, 0, 4, 0, 0, Scene::Forest, 40, 4, Mob::LEFT, "Forest (East)"};
MapSpot mapSpot3 = {4, 457, 274, 3, 0, 0, 0, Scene::Eastville, 1, 1, Mob::RIGHT, "Eastville"};
// Maps spot ID to index in mapSpots.
map<int, int> mapSpotMap;

void OverworldView::init()
{
	currentSpot = 0;
	mapSpotMap[1] = 0;
	mapSpotMap[2] = 1;
	mapSpotMap[3] = 2;
	mapSpotMap[4] = 3;

	mapSpots.push_back(mapSpot0);
	mapSpots.push_back(mapSpot1);
	mapSpots.push_back(mapSpot2);
	mapSpots.push_back(mapSpot3);
};

void OverworldView::setPosition(int pos)
{
	currentSpot = pos;
};

void OverworldView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		MapSpot spot = mapSpots[mapSpotMap[currentSpot]];
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (spot.left != 0) currentSpot = spot.left;
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			if (spot.right != 0) currentSpot = spot.right;
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			if (spot.up != 0) currentSpot = spot.up;
		}
		else if (ev->key.keysym.sym == SDLK_DOWN)
		{
			if (spot.down != 0) currentSpot = spot.down;
		}
		else if (ev->key.keysym.sym == options.confirmKey)
		{
			MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
			state->x = spot.sceneX;
			state->y = spot.sceneY;
			state->orient = spot.sceneOrient;
			state->sceneID = spot.sceneID;
			state->motX = state->motY = state->offX = state->offY = state->steps = 0;
			
			sceneView.setScene(spot.sceneID);
			currentView = &sceneView;
		};
	};
};

void OverworldView::render()
{
	ssOverworld->draw(0, 0, 0, false);

	int mobX, mobY;
	const char *placeName = "Unknown location";

	vector<MapSpot>::iterator it;
	for (it=mapSpots.begin(); it!=mapSpots.end(); ++it)
	{
		ssMapSpot->draw(it->x-12, it->y-12, 0, false);
		if (it->id == currentSpot)
		{
			mobX = it->x-24;
			mobY = it->y-24;
			placeName = it->name;
		};
	};

	GetMobInfo("MOBTODD").mobSprite->draw(mobX, mobY, 0, false);

	Text text(placeName, 0, 0, 0, 255, fntCaption);
	text.draw(mobX+48, mobY+10);
};
