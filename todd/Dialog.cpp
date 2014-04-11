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

Dialog::Dialog(DialogEntry *entry, string with) : currentEntry(entry), with(with), letters(1), choice(0)
{
	timer = Timer::Read();
};

void Dialog::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == SDLK_x)
		{
			if (currentEntry->text != NULL)
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
					choice = 0;
				}
				else
				{
					letters = (int) text.size();
				};
			}
			else
			{
				DialogOption option = currentEntry->options[choice];
				currentEntry = option.effect;
				letters = 1;
				choice = 0;
			};
		}
		else if (ev->key.keysym.sym == SDLK_UP)
		{
			if (currentEntry->text == NULL)
			{
				if (choice != 0) choice--;
			};
		}
		else if (ev->key.keysym.sym == SDLK_DOWN)
		{
			if (currentEntry->text == NULL)
			{
				choice++;
				if (choice == currentEntry->numOptions) choice--;
			};
		};
	};
};

void Dialog::render()
{
	string fxText;
	if (currentEntry->text != NULL)
	{
		fxText = currentEntry->text;
		if ((Timer::Read()-timer) >= 50)
		{
			timer = Timer::Read();
			if (letters < (int) fxText.size()) letters++;
		};
	};

	int y = (SCREEN_HEIGHT*48)-200;
	ssDialog->draw(0, y, 0, false);

	MobInfo info = GetMobInfo(currentEntry->speaker);
	Text cap(info.name, 0, 0, 0, 255, fntCaption);
	cap.draw(241, y+14);

	ssElements->draw(191, y+6, info.element, false);

	if (info.mobSprite != NULL)
	{
		info.mobSprite->draw(70, y+76, 0, false);
	};

	if (currentEntry->text != NULL)
	{
		fxText = fxText.substr(0, letters);
		Text txt(fxText, 0, 0, 0, 255, fntText, 767);
		txt.draw(191, y+62);
	}
	else
	{
		int i;
		int plotY = y+62;
		for (i=0; i<currentEntry->numOptions; i++)
		{
			DialogOption option = currentEntry->options[i];
			Text txt(option.text, 0, 0, 0, 255, fntText, 767);
			txt.draw(217, plotY+2);

			if (i == choice)
			{
				int cx = 191;
				if ((Timer::Read() % 1000) < 500) cx += 2;
				ssCursor->draw(cx, plotY, 0, false);
			};

			plotY += 24;
		};
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

DialogEntry dialFeministRapeCulture[] = {
	{"MOBFEMINIST", "But Rape Culture is very bad :'("},
	{NULL, NULL, NULL}
};

DialogEntry dialEmpty = {NULL, NULL, NULL};

DialogEntry dialFeminist[] = {
	{"MOBFEMINIST", "Stop oppressing me!"},
	{"MOBTODD", "I'm not."},
	{"MOBFEMINIST", "You are!"},
	{"MOBTODD", "Nah."},
	{"MOBFEMINIST", "I hate you so much I'll save the game!", FemSaveGame},
	{"MOBTODD", NULL, NULL, 2, {
		{"K.", &dialEmpty},
		{"I support Rape Culture", dialFeministRapeCulture}
	}},
	{NULL, NULL, NULL}
};
