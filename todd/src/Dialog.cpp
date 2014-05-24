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
#include "EnemyGoblin.h"
#include "EnemyBandit.h"
#include "SaveView.h"
#include "Character.h"
#include "Options.h"

Dialog::Dialog(DialogEntry *entry, string with) : currentEntry(entry), with(with), letters(1), choice(0)
{
	timer = Timer::Read();
};

void Dialog::handleEvent(SDL_Event *ev)
{
	if (ev->type == SDL_KEYDOWN)
	{
		if (ev->key.keysym.sym == options.confirmKey)
		{
			if (currentEntry->mobwait != NULL)
			{
				if (!IsMobReady(currentEntry->mobwait)) return;
			};

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

	int element;
	MobInfo info;
	info.mobSprite = NULL;
	int y = (SCREEN_HEIGHT*48)-200;
	if (currentEntry->speaker != NULL)
	{
		info = GetMobInfo(currentEntry->speaker);
		element = info.element;
	}
	else
	{
		element = Element::NEUTRAL;
	};
	ssDialog->draw(0, y, element, false);

	int red=0, green=0, blue=0;
	if ((element == Element::WATER) || (element == Element::DARKNESS)
		|| (element == Element::NEUTRAL))
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
	bool live = currentEntry->caption != NULL;
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

// FEMINIST
void FemFight()
{
	StartBattle(new EnemyFeminist, new EnemyGoblin, new EnemyGoblin, new EnemyGoblin);
	battleView.setFlee(false);
};

DialogEntry dialFeministRapeCulture[] = {
	{"MOBFEMINIST", "Feminist", "But Rape Culture is very bad :'("},
	{NULL, NULL, NULL}
};

DialogEntry dialFeministFight[] = {
	{"MOBFEMINIST", "Feminist", "Very well! Let's have it broth!", FemFight},
	{"MOBTODD", "Todd", "Haha! I defeated you because you are stupid!"},
	{"MOBFEMINIST", "Feminist", "Rapist :'("},
	{NULL, NULL, NULL}
};

DialogEntry dialEmpty = {NULL, NULL, NULL};

DialogEntry dialFeminist[] = {
	{"MOBFEMINIST", "Feminist", "Stop oppressing me!"},
	{"MOBTODD", "Todd", "I'm not."},
	{"MOBFEMINIST", "Feminist", "You are!"},
	{"MOBTODD", "Todd", "Nah."},
	{"MOBFEMINIST", "Feminist", "Could you please leave? :/"},
	{"MOBTODD", "Todd", NULL, NULL, 3, {
		{"K.", &dialEmpty},
		{"I support Rape Culture", dialFeministRapeCulture},
		{"I want to fight you because I have swag.", dialFeministFight}
	}},
	{NULL, NULL, NULL}
};

// CRYSTAL
void CrystalHeal()
{
	int i;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			chr->setHP(chr->getMaxHP());
			chr->setMP(chr->getMaxMP());
		};
	};
};

void CrystalSave()
{
	saveView.init();
	currentView = &saveView;
};

DialogEntry dialCrystalSave[] = {
	{NULL, "Magic Crystal", "Select a save slot, press RIGHT, enter a name (if needed), and press RIGHT to save.", CrystalSave},
	{NULL, NULL, NULL}
};

DialogEntry dialCrystal[] = {
	{NULL, "Magic Crystal", "You have been fully healed!", CrystalHeal},
	{"MOBTODD", "Todd", NULL, NULL, 2, {
		{"Save...", dialCrystalSave},
		{"Leave...", &dialEmpty}
	}},
	{NULL, NULL, NULL}
};

// LOCKED DOOR
DialogEntry dialLockedDoor[] = {
	{NULL, "Door", "It's locked."},
	{NULL, NULL, NULL}
};

// CASPAR
void CasparJoinParty()
{
	SetPartyMember(1, "CHRCASPAR");
	MobState *state = (MobState*) GetGameData("MOBCASPAR", sizeof(MobState));
	state->sceneID = -1;
};

DialogEntry dialCaspar[] = {
	{"MOBCASPAR", "Caspar", "Good morning, Todd. How are you today?"},
	{"MOBTODD", "Todd", "I'm good. What's going on lately?"},
	{"MOBCASPAR", "Caspar", "We have received reports that the Chief of Eastville was kidnapped. We need to go there and intervene."},
	{"MOBCASPAR", "Caspar", "We can get to there through the forest to the east. We need to be careful since the goblins are getting pretty aggressive recently!"},
	{"MOBTODD", "Todd", "OK, let's go!", CasparJoinParty},
	{NULL, NULL, NULL}
};

// MAN IN THE FOREST THAT BLOCKS PATH UNTIL YOU TALK TO CASPAR
DialogEntry dialManForest1WithoutCaspar[] = {
	{"MOBMANFOREST1", "Man", "Sorry, I can't let you go through here sir, we're patrolling the area in search of bandits who are robbing passers-by."},
	{NULL, NULL, NULL}
};

void ManForest1MoveAway()
{
	EnqueueMobMoves("MOBMANFOREST1", Mob::LEFT, 2);
	EnqueueMobMoves("MOBMANFOREST1", Mob::UP, 5);
};

DialogEntry dialManForest1WithCaspar[] = {
	{"MOBMANFOREST1", "Man", "Sorry, I can't let you go through here, we're patrolling the area in search of bandits who are robbing passers-by."},
	{"MOBCASPAR", "Caspar", "We're in the Royal Guard, looking for the Chief of Eastville who was kidnapped. I command you to let us through. We'll take care of the bandits ourselves.", ManForest1MoveAway},
	{"MOBMANFOREST1", "Man", "Yes, sir!", NULL, 0, {}, "MOBMANFOREST1"},
	{NULL, NULL, NULL}
};

// MAN IN THE FOREST THAT BLOCKS THE MYSTERIOUS PATH
DialogEntry dialManForest2[] = {
	{"MOBMANFOREST2", "Man", "Sorry, nobody allowed past this point! There's secret government operations going on there!"},
	{NULL, NULL, NULL}
};

// BANDIT BOSS (IN THE FOREST)
void BanditBossBattle()
{
	MobState *state = (MobState*) GetGameData("MOBBANDIT", sizeof(MobState));
	state->sceneID = -1;

	StartBattle(new EnemyBandit());
};

DialogEntry dialBanditBoss[] = {
	{"MOBTODD", "Todd", "Hey! Who are you?"},
	{"MOBBANDIT", "Bandit", "Give me all your money!"},
	{"MOBCASPAR", "Caspar", "Come here and get it then!", BanditBossBattle},
	{NULL, NULL, NULL}
};
