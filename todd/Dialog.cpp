/**
 * Dialog.cpp
 * The "dialog" GUI, which displays people talking.
 */

#include "Dialog.h"
#include "GameState.h"
#include "Mob.h"
#include "SpriteSheet.h"
#include "Todd.h"
#include "Text.h"
#include "Timer.h"

Dialog::Dialog(DialogEntry *entry, string with) : currentEntry(entry), with(with), letters(1)
{
	timer = Timer::Read();
};

void Dialog::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_x)
		{
			string text = currentEntry->text;
			if (letters == (int) text.size())
			{
				if (currentEntry->callback != NULL)
				{
					currentEntry->callback();
				};

				currentEntry++;
				letters = 1;
			}
			else
			{
				letters = (int) text.size();
			};
		};
	};
};

void Dialog::render()
{
	string fxText = currentEntry->text;

	if ((Timer::Read()-timer) >= 50)
	{
		timer = Timer::Read();
		if (letters < (int) fxText.size()) letters++;
	};

	int y = (SCREEN_HEIGHT*48)-200;
	ssDialog->draw(0, y, 0, false);

	MobInfo info = GetMobInfo(currentEntry->speaker);
	Text cap(info.name, 0, 0, 0, 255, fntCaption);
	cap.draw(241, y+14);

	ssElements->draw(191, y+6, info.element, false);

	fxText = fxText.substr(0, letters);
	Text txt(fxText, 0, 0, 0, 255, fntText, 767);
	txt.draw(191, y+62);

	if (info.mobSprite != NULL)
	{
		info.mobSprite->draw(70, y+76, 0, false);
	};
};

bool Dialog::isLiving()
{
	bool live = currentEntry->speaker != NULL;
	if (!live)
	{
		if (with != "")
		{
			MobState *state = (MobState*) GetGameData(with, sizeof(MobState));
			state->lock = 0;
		};
	};
	return live;
};

void FemSaveGame()
{
	SaveGame(3);
};

DialogEntry dialFeminist[] = {
	{"MOBFEMINIST", "Stop oppressing me!"},
	{"MOBTODD", "I'm not."},
	{"MOBFEMINIST", "You are!"},
	{"MOBTODD", "Nah."},
	{"MOBFEMINIST", "I hate you so much I'll save the game!", FemSaveGame},
	{"MOBTODD", "K."},
	{"MOBFEMINIST", " Because I am a woman, I must make unusual efforts to succeed. If I fail, no one will say, 'She doesn't have what it takes.' They will say, 'Women don't have what it takes.' ~Clare Boothe Luce"},
	{NULL, NULL, NULL}
};
