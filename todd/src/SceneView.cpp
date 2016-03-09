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
#include "Character.h"

#ifdef TODD_DEBUG
#include "DebugOptionsView.h"
#endif

/**
 * The magic boundary - the player must be at least this many pixels from the border.
 * If they're not, the camera moves to make them.
 */
#define	MAGIC_BOUND			(48*5)

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
	{5, 0},				// Save crystal (active tile, use in hard layer)
	{6, 7},				// Warp to overworld
	{7, 8},				// Wooden floor
	{8, 0},				// Door
	{9, 0},				// Locked door
	{10, 13},			// Grass with a rock on it
	{11, 0},			// Hidden door
	{12, 29},			// Sand
	{13, 30},			// Shadow sand (the sandy stuff in the shadow realm)
	{14, 12},			// Water
	{15, 31},			// Shadow temple bricks
	{16, 32},			// Shadow temple door
	{17, 34},			// Shadow temple floor
	{18, 36},			// Wooden bridge
	
	// The house strcuture (19-60 inclusive)
	{19, 160},
	{26, 176},
	{33, 192},
	{40, 208},
	{47, 224},
	{20, 161},
	{27, 177},
	{34, 193},
	{41, 209},
	{48, 225},
	{21, 162},
	{28, 178},
	{35, 194},
	{42, 210},
	{49, 226},
	{22, 163},
	{29, 179},
	{36, 195},
	{43, 211},
	{50, 227},
	{23, 164},
	{30, 180},
	{37, 196},
	{44, 212},
	{51, 228},
	{24, 165},
	{31, 181},
	{38, 197},
	{45, 213},
	{52, 229},
	{25, 166},
	{32, 182},
	{39, 198},
	{46, 214},
	{53, 230},
	{54, 240},
	{55, 241},
	{56, 242},
	{57, 243},
	{58, 244},
	{59, 245},
	{60, 246},

	// decorative
	{61, 14},
	{62, 15},
	{63, 16},
	{64, 17},
	{65, 18},
	{66, 19},
	{67, 20},
	{68, 21},
	{69, 22},
	{70, 23},
	{71, 24},
	{72, 25},
	{73, 26},
	{74, 27},
	{75, 28},
	
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

extern map<string, SpriteSheet*> mobSprites;
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

	// Adjust the camera so we can see the player's character.
	state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	int realX = state->x*48+state->offX;
	int realY = state->y*48+state->offY;

	if (realX < (cameraX+MAGIC_BOUND)) cameraX = realX - MAGIC_BOUND;
	if (realX > (cameraX+(SCREEN_WIDTH*48)-MAGIC_BOUND)) cameraX = realX - (SCREEN_WIDTH*48) + MAGIC_BOUND;
	if (realY < (cameraY+MAGIC_BOUND)) cameraY = realY - MAGIC_BOUND;
	if (realY > (cameraY+(SCREEN_HEIGHT*48)-MAGIC_BOUND)) cameraY = realY - (SCREEN_HEIGHT*48) + MAGIC_BOUND;

	if (cameraX < 0) cameraX = 0;
	if (cameraX > (48*(sceneView.width()-SCREEN_WIDTH))) cameraX = 48*(sceneView.width()-SCREEN_WIDTH);
	if (cameraY < 0) cameraY = 0;
	if (cameraY > (48*(sceneView.height()-SCREEN_HEIGHT))) cameraY = (48*(sceneView.height()-SCREEN_HEIGHT));
	
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
