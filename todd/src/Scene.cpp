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
 * Scene.cpp
 * Manages a scene.
 */

#include "Scene.h"
#include "Todd.h"
#include <fstream>
#include <string.h>
#include <map>
#include "Mob.h"
#include "GameState.h"
#include "SceneView.h"
#include "OverworldView.h"
#include "Struct.h"

using namespace std;

// Maps scene IDs to scenes.
map<int, Scene*> scenes;

void Scene::LoadScenes()
{
	scenes[Forest] = new Scene("forest.scn");
	scenes[Castle_ToddRoom] = new Scene("castle_todd_room.scn");
	scenes[Castle] = new Scene("castle.scn");
	scenes[Eastville] = new Scene("eastville.scn");
	scenes[Eastville_House1] = new Scene("eastville_h1.scn");
	scenes[Eastville_PotionShop] = new Scene("eastville_ps.scn");
	scenes[ShadowRealm] = new Scene("shadow_realm.scn");
	scenes[ShadowTemple] = new Scene("shadow_temple.scn");
};

Scene* Scene::GetSceneByID(int id)
{
	return scenes[id];
};

void Scene::processTile(vector<string> &lines, int x, int y, Tile *layer, map<char, Tile> &tileMap)
{
	if (y < (int) lines.size())
	{
		string line = lines[y];
		if (x < (int) line.size())
		{
			char c = line[x];
			layer[y * width + x] = tileMap[c];
		};
	};
};

Scene::Scene(string name)
{
	string path = GetResourcePath(name);
	ifstream ifs(path.c_str());
	if (ifs.fail())
	{
		GameAbort(string("Could not open scene file ") + name);
	};

	warpPoints.clear();
	structList.clear();

	vector<string> bgLines;
	vector<string> hardLines;
	vector<string> ovLines;
	map<char, Tile> tileMap;
	int bgWidth=0, hardWidth=0, ovWidth=0;

	int *ptrWidth = &bgWidth;
	vector<string> *ptrLines = &bgLines;

	string line;
	while (getline(ifs, line))
	{
		if (line[0] == '#') continue;
		if (line == "") continue;

		if (line[0] == '.')
		{
			string cmd, param;
			size_t pos = line.find(' ');
			if (pos == line.npos)
			{
				cmd = line;
				param = "";
			}
			else
			{
				cmd = line.substr(0, pos);
				param = line.substr(pos+1);
			};

			if (cmd == ".bg")
			{
				ptrLines = &bgLines;
				ptrWidth = &bgWidth;
			}
			else if (cmd == ".hard")
			{
				ptrLines = &hardLines;
				ptrWidth = &hardWidth;
			}
			else if (cmd == ".overlay")
			{
				ptrLines = &ovLines;
				ptrWidth = &ovWidth;
			}
			else if (cmd == ".warp")
			{
				size_t colonPos = param.find(":");
				if (colonPos == param.npos)
				{
					GameAbort(string("Error loading scene ") + name + ": invalid syntax for .warp");
				};

				string strTarget = param.substr(colonPos+1);
				string coordParse = param.substr(0, colonPos);
				size_t commaPos = coordParse.find(",");
				if (commaPos == coordParse.npos)
				{
					GameAbort(string("Error loading scene ") + name + ": invalid syntax for .warp");
				};

				string strX = coordParse.substr(0, commaPos);
				string strY = coordParse.substr(commaPos+1);

				WarpPoint point;
				if (1)
				{
					stringstream ss;
					ss << strX;
					ss >> point.x;
				};

				if (1)
				{
					stringstream ss;
					ss << strY;
					ss >> point.y;
				};

				if (1)
				{
					stringstream ss;
					ss << strTarget;
					ss >> point.targetScene;
				};

				warpPoints.push_back(point);
			}
			else if (cmd == ".tile")
			{
				char c = param[0];
				int tile, meta;
				string rest = param.substr(1);

				size_t pos = rest.find(":");
				if (pos == rest.npos)
				{
					stringstream ss;
					ss << rest;
					ss >> tile;
					meta = 0;
				}
				else
				{
					string a = rest.substr(0, pos);
					string b = rest.substr(pos+1);

					if (1)
					{
						stringstream ss;
						ss << a;
						ss >> tile;
					};

					if (1)
					{
						stringstream ss;
						ss << b;
						ss >> meta;
					};
				};

				Tile tl;
				tl.id = tile;
				tl.meta = meta;
				tileMap[c] = tl;
			}
			else if (cmd == ".struct")
			{
				size_t colonPos = param.find(":");
				if (colonPos == param.npos)
				{
					GameAbort(string("Error loading scene ") + name + ": invalid syntax for .struct");
				};

				string name = param.substr(colonPos+1);
				string coordParse = param.substr(0, colonPos);
				size_t commaPos = coordParse.find(",");
				if (commaPos == coordParse.npos)
				{
					GameAbort(string("Error loading scene ") + name + ": invalid syntax for .struct");
				};

				string strX = coordParse.substr(0, commaPos);
				string strY = coordParse.substr(commaPos+1);

				StructInfo info;
				if (1)
				{
					stringstream ss;
					ss << strX;
					ss >> info.x;
				};

				if (1)
				{
					stringstream ss;
					ss << strY;
					ss >> info.y;
				};

				info.str = GetStruct(name);
				structList.push_back(info);
			}
			else
			{
				GameAbort(string("Error loading scene ") + name + ": invalid command encountered: " + cmd);
			};
		}
		else
		{
			if ((int)line.size() > (*ptrWidth))
			{
				*ptrWidth = (int) line.size();
			};

			ptrLines->push_back(line);
		};
	};

	ifs.close();

	// Decide the width and height of the full scene.
	width = bgWidth;
	if (hardWidth > width) width = hardWidth;
	if (ovWidth > width) width = ovWidth;

	height = (int) bgLines.size();
	if ((int) hardLines.size() > height) height = (int) hardLines.size();
	if ((int) ovLines.size() > height) height = (int) ovLines.size();

	// Initialize the arrays
	bgLayer = new Tile[width * height];
	hardLayer = new Tile[width * height];
	ovLayer = new Tile[width * height];

	memset(bgLayer, 0, sizeof(Tile)*width*height);
	memset(hardLayer, 0, sizeof(Tile)*width*height);
	memset(ovLayer, 0, sizeof(Tile)*width*height);

	// Now process all the layers
	int x, y;
	for (x=0; x<width; x++)
	{
		for (y=0; y<height; y++)
		{
			processTile(bgLines, x, y, bgLayer, tileMap);
			processTile(hardLines, x, y, hardLayer, tileMap);
			processTile(ovLines, x, y, ovLayer, tileMap);
		};
	};
};

bool Scene::checkWarp(int relX, int relY)
{
	MobState *state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	int targetX = state->x + relX;
	int targetY = state->y + relY;

	vector<WarpPoint>::iterator it;
	for (it=warpPoints.begin(); it!=warpPoints.end(); ++it)
	{
		if ((it->x == targetX) && (it->y == targetY))
		{
			if (it->targetScene < 0)
			{
				overworldView.setPosition(-it->targetScene);
				currentView = &overworldView;
			}
			else
			{
				state->sceneID = it->targetScene;
				sceneView.setScene(it->targetScene);
			};

			return true;
		};
	};

	return false;
};

Scene::~Scene()
{
	delete[] bgLayer;
	delete[] hardLayer;
	delete[] ovLayer;
};
