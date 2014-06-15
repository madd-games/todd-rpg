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
#include "ActiveTile.h"
#include "RandomBattle.h"
#include "BattleView.h"
#include "Timer.h"
#include "Options.h"

#ifdef TODD_DEBUG
#include "DebugOptionsView.h"
#endif

using namespace std;

SceneView sceneView;

struct TileMapping
{
	int id;
	int spriteIndex;
};

/**
 * \brief Maps tile IDs to tile sprite indeces.
 */
TileMapping tileMappings[] = {
	{1, 1},				// Black square
	{2, 2},				// Grass
	{3, 3},				// Tree (use in hard layer)
	{4, 0},				// Chest (active tile)
	{5, 0},				// Save crystal (active tile, use in hard layer).
	{6, 7},				// Warp to overworld
	{7, 8},				// Wooden floor
	{8, 0},				// Door
	{9, 0},				// Locked door
	{10, 13},			// Grass with a rock on it.
	{11, 0},			// Hidden door.
	// END
	{0, 0}
};

SceneView::SceneView() : keyLeft(false), keyRight(false), keyUp(false), keyDown(false), gui(NULL), stepCount(0),
				battleTimer(0), ghostWalk(false), enableRandomBattles(true)
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

			ActiveTile *atile = GetActiveTile(tile.id);
			if (atile != NULL)
			{
				void *state = GetTileState(sceneID, x, y, atile);
				spriteIndex = atile->getTileIndex(state);
			};

			if (spriteIndex == 0)
			{
				while (scan->id != 0)
				{
					if (scan->id == tile.id)
					{
						spriteIndex = scan->spriteIndex;
						break;
					};

					scan++;
				};
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
			keyDown = !scene->checkWarp(0, 1);
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			keyUp = !scene->checkWarp(0, -1);
		}
		else if (ev->key.keysym.sym == SDLK_LEFT)
		{
			keyLeft = !scene->checkWarp(-1, 0);
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			keyRight = !scene->checkWarp(1, 0);
		}
		else if (ev->key.keysym.sym == options.confirmKey)
		{
			if (keyLeft || keyRight || keyUp || keyDown) return;

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
			}
			else
			{
				int id = scene->hardLayer[(state->y+relY) * scene->width + (state->x+relX)].id;
				ActiveTile *tile = GetActiveTile(id);
				if (tile != NULL)
				{
					void *tstate = GetTileState(sceneID, state->x+relX, state->y+relY, tile);
					tile->interact(tstate);
				};
			};
		}
#ifdef TODD_DEBUG
		else if (ev->key.keysym.sym == SDLK_F6)
		{
			currentView = &debugOptionsView;
		}
#endif
		else if (ev->key.keysym.sym == options.menuKey)
		{
			gui = new GameMenu();
		};
	}
	else if (ev->type == SDL_KEYUP)
	{
		if (ev->key.keysym.sym == options.downKey)
		{
			keyDown = false;
		}
		else if (ev->key.keysym.sym == options.upKey)
		{
			keyUp = false;
		}
		else if (ev->key.keysym.sym == options.leftKey)
		{
			keyLeft = false;
		}
		else if (ev->key.keysym.sym == options.rightKey)
		{
			keyRight = false;
		};
	};
};

void SceneView::render()
{
	if (enableRandomBattles)
	{
		if (battleTimer != 0)
		{
			if ((Timer::Read()-battleTimer) >= 1000)
			{
				battleTimer = 0;
				StartBattle(randomBattle.a, randomBattle.b, randomBattle.c, randomBattle.d);
				return;
			};
		};
	};

	if (gui != NULL)
	{
		if (!gui->isLiving())
		{
			delete gui;
			gui = NULL;
		};
	};

	MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	if ((state->steps == 0) && (battleTimer == 0))
	{
		bool move = keyLeft || keyRight || keyUp || keyDown;
		if (move)
		{
			stepCount++;
			if (stepCount >= 15)
			{
				if (GetRandomBattle(sceneID, randomBattle) && enableRandomBattles)
				{
					stepCount = 0;
					battleTimer = Timer::Read();
					return;
				};
			};

			if (keyLeft)
			{
				state->beginMove(Mob::LEFT, ghostWalk);
			}
			else if (keyRight)
			{
				state->beginMove(Mob::RIGHT, ghostWalk);
			}
			else if (keyUp)
			{
				state->beginMove(Mob::UP, ghostWalk);
			}
			else
			{
				state->beginMove(Mob::DOWN, ghostWalk);
			};
		};
	};

	UpdateMobs();
	renderLayer(scene->bgLayer);
	renderLayer(scene->hardLayer);
	RenderMobs();
	renderLayer(scene->ovLayer);

	vector<Scene::StructInfo>::iterator it;
	for (it=scene->structList.begin(); it!=scene->structList.end(); ++it)
	{
		it->str->draw(it->x*48, it->y*48);
	};

	if (battleTimer != 0)
	{
		ssTiles->draw(state->x*48, state->y*48-48, 10);
	};

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

	vector<Scene::StructInfo>::iterator it;
	for (it=scene->structList.begin(); it!=scene->structList.end(); ++it)
	{
		if ((x >= it->x) && (y >= it->y))
		{
			int relX = x - it->x;
			int relY = y - it->y;

			if ((relX < it->str->getWidth()) && (relY < it->str->getHeight()))
			{
				if (!it->str->canWalkOn(relX, relY))
				{
					return false;
				};
			};
		};
	};

	return true;
};

void SceneView::openDialog(DialogEntry *entry, string with)
{
	openGUI(new Dialog(entry, with));
};

void SceneView::openGUI(GUI *gui)
{
	if (this->gui != NULL)
	{
		delete this->gui;
	};

	this->gui = gui;
};

int SceneView::width()
{
	return scene->width;
};

int SceneView::height()
{
	return scene->height;
};
