/**
 * SceneView.cpp
 * The "scene" view, where the player can walk around a scene.
 */

#include "SceneView.h"
#include "Todd.h"
#include "SpriteSheet.h"
#include "Mob.h"
#include <iostream>
#include "GameState.h"
#include "GameMenu.h"

using namespace std;

SceneView sceneView;

struct TileMapping
{
	int id;
	int spriteIndex;

	/**
	 * The "second" sprite index, if the tile is animated.
	 */
	int secondSpriteIndex;
};

TileMapping tileMappings[] = {
	{1, 1, 1},			// Grass
	{2, 2, 2},			// ??? (don't know yet)
	{3, 3, 3},			// Tree (use in hard layer)

	// END
	{0, 0}
};

SceneView::SceneView() : keyLeft(false), keyRight(false), keyUp(false), keyDown(false), gui(NULL)
{
};

void SceneView::renderLayer(Scene::Tile *layer)
{
	int x, y;
	for (x=0; x<scene->width; x++)
	{
		for (y=0; y<scene->height; y++)
		{
			Scene::Tile tile = layer[y * scene->width + x];
			TileMapping *scan = tileMappings;
			if (tile.id == 0) continue;
			int spriteIndex = 0;

			while (scan->id != 0)
			{
				if (scan->id == tile.id)
				{
					spriteIndex = scan->spriteIndex;
					break;
				};

				scan++;
			};

			ssTiles->draw(x*48, y*48, spriteIndex);
		};
	};
};

void SceneView::handleEvent(SDL_Event *ev)
{
	if (gui != NULL)
	{
		gui->handleEvent(ev);
		return;
	};

	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_DOWN)
		{
			keyDown = true;
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			keyUp = true;
		}
		else if (ev->key.keysym.sym == SDLK_LEFT)
		{
			keyLeft = true;
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			keyRight = true;
		}
		else if (ev->key.keysym.sym == SDLK_x)
		{
			int relX=0, relY=0, opposite=0;
			MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));

			if (state->orient == Mob::LEFT)
			{
				relX = -1;
				opposite = Mob::RIGHT;
			}
			else if (state->orient == Mob::RIGHT)
			{
				relX = 1;
				opposite = Mob::LEFT;
			}
			else if (state->orient == Mob::UP)
			{
				relY = -1;
				opposite = Mob::DOWN;
			}
			else
			{
				relY = 1;
				opposite = Mob::UP;
			};

			string name = GetMobAt(sceneView.getScene(), state->x+relX, state->y+relY);
			if (name != "")
			{
				MobState *mob = (MobState*) GetGameData(name, sizeof(MobState));
				mob->orient = opposite;
				InteractWithMob(name);
			};
		}
		else if (ev->key.keysym.sym == SDLK_ESCAPE)
		{
			gui = new GameMenu();
		};
	}
	else if (ev->type == SDL_KEYUP)
	{
		if (ev->key.keysym.sym == SDLK_DOWN)
		{
			keyDown = false;
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			keyUp = false;
		}
		else if (ev->key.keysym.sym == SDLK_LEFT)
		{
			keyLeft = false;
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			keyRight = false;
		};
	};
};

void SceneView::render()
{
	if (gui != NULL)
	{
		if (!gui->isLiving())
		{
			delete gui;
			gui = NULL;
		};
	};

	MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	if (state->steps == 0)
	{
		bool move = keyLeft || keyRight || keyUp || keyDown;
		if (move)
		{
			if (keyLeft)
			{
				state->beginMove(Mob::LEFT);
			}
			else if (keyRight)
			{
				state->beginMove(Mob::RIGHT);
			}
			else if (keyUp)
			{
				state->beginMove(Mob::UP);
			}
			else
			{
				state->beginMove(Mob::DOWN);
			};
		};
	};

	UpdateMobs();
	renderLayer(scene->bgLayer);
	renderLayer(scene->hardLayer);
	RenderMobs();
	renderLayer(scene->ovLayer);

	if (gui != NULL)
	{
		gui->render();
	};
};

void SceneView::setScene(int id)
{
	this->scene = Scene::GetSceneByID(id);
	this->sceneID = id;
};

int SceneView::getScene()
{
	return sceneID;
};

bool SceneView::canWalk(int x, int y)
{
	if ((x < 0) || (x >= scene->width) || (y < 0) || (y >= scene->height)) return false;
	if (scene->hardLayer[y * scene->width + x].id != 0) return false;
	if (IsMobAt(sceneID, x, y)) return false;
	return true;
};

void SceneView::openDialog(DialogEntry *entry, string with)
{
	gui = new Dialog(entry, with);
};

int SceneView::width()
{
	return scene->width;
};

int SceneView::height()
{
	return scene->height;
};
