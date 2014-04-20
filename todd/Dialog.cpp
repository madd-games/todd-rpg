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
#include "BattleView.h"
#include "EnemyFeminist.h"

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

	MobInfo info = GetMobInfo(currentEntry->speaker);
	int y = (SCREEN_HEIGHT*48)-200;
	ssDialog->draw(0, y, info.element, false);

	int red=0, green=0, blue=0;
	if ((info.element == Element::WATER) || (info.element == Element::DARKNESS)
		|| (info.element == Element::NEUTRAL))
	{
		red = 255;
		green = 255;
		blue = 255;
	};
	Text cap(currentEntry->caption, red, green, blue, 255, fntCaption);
	cap.draw(191, y+14);

	if (info.mobSprite != NULL)
	{
		info.mobSprite->draw(70, y+76, 0, false);
	};

	if (currentEntry->text != NULL)
	{
		fxText = fxText.substr(0, letters);
		Text txt(fxText, red, green, blue, 255, fntText, 767);
		txt.draw(191, y+62);
	}
	else
	{
		int i;
		int plotY = y+62;
		for (i=0; i<currentEntry->numOptions; i++)
		{
			DialogOption option = currentEntry->options[i];
			Text txt(option.text, red, green, blue, 255, fntText, 767);
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

void FemFight()
{
	StartBattle(new EnemyFeminist);
};

DialogEntry dialFeministRapeCulture[] = {
	{"MOBFEMINIST", "Feminist", "But Rape Culture is very bad :'("},
	{NULL, NULL, NULL}
};

DialogEntry dialFeministFight[] = {
	{"MOBFEMINIST", "Feminist", "Very well! Let's have it broth!", FemFight},
	{NULL, NULL, NULL}
};

DialogEntry dialEmpty = {NULL, NULL, NULL};

DialogEntry dialFeminist[] = {
	{"MOBFEMINIST", "Feminist", "Stop oppressing me!"},
	{"MOBTODD", "Todd", "I'm not."},
	{"MOBFEMINIST", "Feminist", "You are!"},
	{"MOBTODD", "Todd", "Nah."},
	{"MOBFEMINIST", "Feminist", "I hate you so much I'll save the game!", FemSaveGame},
	{"MOBTODD", "Todd", NULL, NULL, 3, {
		{"K.", &dialEmpty},
		{"I support Rape Culture", dialFeministRapeCulture},
		{"I want to fight you because I have swag.", dialFeministFight}
	}},
	{NULL, NULL, NULL}
};
