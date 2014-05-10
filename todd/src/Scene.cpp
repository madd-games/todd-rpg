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

using namespace std;

// Maps scene IDs to scenes.
map<int, Scene*> scenes;

void Scene::LoadScenes()
{
	scenes[Test] = new Scene("test.scn");
	scenes[Forest] = new Scene("forest.scn");
	scenes[Castle_ToddRoom] = new Scene("castle_todd_room.scn");
	scenes[Castle] = new Scene("castle.scn");
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
