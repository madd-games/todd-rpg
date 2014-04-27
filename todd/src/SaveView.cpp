/**
 * SaveView.cpp
 * The "save game" view.
 */

#include "SaveView.h"
#include "MainMenu.h"
#include "GameState.h"
#include <fstream>
#include <sstream>
#include "Text.h"
#include "Todd.h"
#include "SpriteSheet.h"
#include "Timer.h"
#include "Mob.h"
#include "SceneView.h"

SaveView saveView;

void SaveView::init()
{
	slotSel = 1;
	slotNames.clear();
	saving = false;
	int i;
	for (i=1; i<17; i++)
	{
		string path = GetPathToSaveSlot(i);
		ifstream ifs(path.c_str(), ios::binary);
		if (!ifs.fail())
		{
			SaveHeader head;
			ifs.read((char*)&head, sizeof(SaveHeader));
			ifs.close();

			char buf[33];
			buf[32] = 0;
			memcpy(buf, head.name, 32);

			slotNames.push_back(buf);
		}
		else
		{
			slotNames.push_back("EMPTY SLOT");
		};
	};
};

void SaveView::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (!saving)
		{
			if ((ev->key.keysym.sym == SDLK_z) || (ev->key.keysym.sym == SDLK_LEFT))
			{
				currentView = &sceneView;
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				if (slotSel != 16) slotSel++;
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (slotSel != 0) slotSel--;
			}
			else if ((ev->key.keysym.sym == SDLK_RIGHT) || (ev->key.keysym.sym == SDLK_x))
			{
				saving = true;
				saveName = slotNames[slotSel-1];
			};
		}
		else
		{
			if (ev->key.keysym.sym == SDLK_LEFT)
			{
				saving = false;
			}
			else if (ev->key.keysym.sym == SDLK_RIGHT)
			{
				slotNames[slotSel-1] = saveName;
				SaveGame(slotSel, saveName);
				saving = false;
			}
			else if (ev->key.keysym.sym < 256)
			{
				char c = (char) ev->key.keysym.sym;
				if ((c >= 32) && (c <= 126))
				{
					if (saveName.size() < 32)
					{
						saveName += c;
					};
				}
				else if (c == '\b')
				{
					if (saveName.size() > 0)
					{
						saveName = saveName.substr(0, saveName.size()-1);
					};
				};
			};
		};
	};
};

void SaveView::render()
{
	Text txtSaveGame("SAVE GAME", 255, 255, 255, 255, fntCaption);
	txtSaveGame.draw(480, 2, Text::CENTER);

	int plotY = 64;
	int i;
	for (i=1; i<17; i++)
	{
		string label;
		int red, green, blue;

		if ((!saving) || (slotSel != i))
		{
			stringstream ss;
			ss << i << ". " << slotNames[i-1];
			label = ss.str();
			red = green = blue = 255;
		}
		else
		{
			label = saveName;
			red = 0;
			green = 0;
			blue = 255;
		};

		Text text(label, red, green, blue, 255, fntText);
		text.draw(24, plotY+2);

		if (i == slotSel)
		{
			int x = 2;
			if (((Timer::Read() % 1000) < 500) || saving) x += 2;
			ssCursor->draw(x, plotY, 0, false);
		};

		plotY += 24;
	};
};
