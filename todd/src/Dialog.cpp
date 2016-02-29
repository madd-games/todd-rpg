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
#include "EnemyGoblin.h"
#include "EnemyBandit.h"
#include "EnemyShadowWarrior.h"
#include "EnemyShadowPriest.h"
#include "EnemyShadowNecromancer.h"
#include "SaveView.h"
#include "Character.h"
#include "Options.h"
#include "Quest.h"
#include "SceneView.h"
#include "Scene.h"

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
		|| (element == Element::NEUTRAL) || (element == Element::EARTH))
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

DialogEntry dialEmpty = {NULL, NULL, NULL};

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
			chr->getStatusEffectSet().clear();
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

	Quest *quest = GetQuest("QSTCHEASTV");
	quest->setStatus(Quest::Active);
};

DialogEntry dialCaspar[] = {
	{"MOBCASPAR", "Caspar", "Good morning, Todd. How are you today?"},
	{"MOBTODD", "Todd", "I'm good. What's going on lately?"},
	{"MOBCASPAR", "Caspar", "We have received reports that the Chief of Eastville was kidnapped. We need to go there and intervene."},
	{"MOBCASPAR", "Caspar", "We can get to there through the forest to the east. We need to be careful since the goblins are getting pretty aggressive recently!"},
	{"MOBCASPAR", "Caspar", "Once we get there, we must try to speak to people and find some witnesses to lead us to the Chief."},
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
	{"MOBMANFOREST1", "Man", "Sorry, I can't let you go through here sir, we're patrolling the area in search of bandits who are robbing passers-by."},
	{"MOBCASPAR", "Caspar", "We're in the Royal Guard, looking for the Chief of Eastville who was kidnapped. I command you to let us through. We'll take care of the bandits ourselves.", ManForest1MoveAway},
	{"MOBMANFOREST1", "Man", "Yes, sir!", NULL, 0, {}, "MOBMANFOREST1"},
	{NULL, NULL, NULL}
};

// MAN IN THE FOREST THAT BLOCKS THE MYSTERIOUS PATH
DialogEntry dialManForest2[] = {
	{"MOBMANFOREST2", "Man", "Sorry, nobody allowed past this point! There's secret government operations going on there!"},
	{NULL, NULL, NULL}
};

void ManForest2Move()
{
	EnqueueMobMoves("MOBMANFOREST2", Mob::LEFT, 2);
	EnqueueMobMoves("MOBMANFOREST2", Mob::DOWN, 4);
};

void ManForest2Die()
{
	MobState *state = (MobState*) GetGameData("MOBMANFOREST2", sizeof(MobState));
	state->sceneID = -1;
};

DialogEntry dialManForest2b[] = {
	{"MOBMANFOREST2", "Man", "The government operations are just finishing. I will now allow you to pass.", ManForest2Move},
	{"MOBMANFOREST2", "Man", "Have a nice day guys.", ManForest2Die, 0, {}, "MOBMANFOREST2"},
	{NULL, NULL, NULL}
};

// BANDIT BOSS (IN THE FOREST)
void BanditBossBattle()
{
	MobState *state = (MobState*) GetGameData("MOBBANDIT", sizeof(MobState));
	state->sceneID = -1;

	StartBattle(new EnemyBandit());
	battleView.setFlee(false);
};

DialogEntry dialBanditBoss[] = {
	{"MOBTODD", "Todd", "Hey! Who are you?"},
	{"MOBBANDIT", "Bandit", "Give me all your money!"},
	{"MOBCASPAR", "Caspar", "Come here and get it then!", BanditBossBattle},
	{NULL, NULL, NULL}
};

// MAN IN EASTVILLE WHO IS WITNESS #1
DialogEntry dialManEastville1a[] = {
	{"MOBMANEASTV1", "George", "Ah, hello! My name is George! You must be soldiers of the Royal Guard."},
	{"MOBTODD", "Todd", "Indeed. We are looking for the missing Chief. Do you know anything about what has happened?"},
	{"MOBMANEASTV1", "George", "Last night I saw some men dressed in black going towards the Chief's home. I'm not sure, but I think they went towards the forest."},
	{"MOBCASPAR", "Caspar", "That does seem very suspicious. Come on, Todd, we must go and see if they're still there!"},
	{"MOBMANEASTV1", "George", "Before you go, I'd suggest you get some equipment! You can take everything from these chests . You can also use my Save Crystal."},
	{"MOBTODD", "Todd", "Thank you!"},
	{NULL, NULL, NULL}
};

DialogEntry dialManEastville1b[] = {
	{"MOBMANEASTV1", "George", "You can take all my equipment! Go search for those bandits, they are probably in the forest!"},
	{NULL, NULL, NULL}
};

// BOB
void BobQuestStart()
{
	Quest *quest = GetQuest("QSTGOBLINDUST");
	quest->setStatus(Quest::Active);
};

DialogEntry dialBob_a[] = {
	{"MOBBOB", "Bob", "Hey, are you from the Royal Guard?"},
	{"MOBCASPAR", "Caspar", "Yes we are."},
	{"MOBBOB", "Bob", "Looking for the chief, aren't you? Last night I saw some dark figures going west, perhaps with the Chief. I suspect they may have taken him to the Shadow Realm."},
	{"MOBCASPAR", "Caspar", "The Shadow Realm?"},
	{"MOBBOB", "Bob", "It is a secret place in Farewell Forest, which can be accessed using the Spirit Key, through the Demonic Stone."},
	{"MOBCASPAR", "Caspar", "How can we get the Spirit Key?"},
	{"MOBBOB", "Bob", "I need you to fight some Goblins and get 10x Goblin Dust. I have all the other ingredients ready.", BobQuestStart},
	{"MOBBOB", "Bob", "Once you find it, I will join you and open the Shadow Realm."},
	{NULL, NULL, NULL}
};

void BobJoin()
{
	SetPartyMember(2, "CHRBOB");
	MobState *state = (MobState*) GetGameData("MOBBOB", sizeof(MobState));
	state->sceneID = -1;

	Quest *quest = GetQuest("QSTGOBLINDUST");
	quest->setStatus(Quest::Complete);
};

DialogEntry dialBob_b_no[] = {
	{"MOBBOB", "Bob", "Sorry, that's not enough Goblin Dust. I need 10 units of it."},
	{NULL, NULL, NULL}
};

DialogEntry dialBob_b_yes[] = {
	{"MOBTODD", "Todd", "Okay, we've got enough Goblin Dust, here you go."},
	{"MOBBOB", "Bob", "Ah, great! I can now make the Spirit Key."},
	{"MOBBOB", "Bob", "Now let's go to the Demonic Stone!", BobJoin},
	{NULL, NULL, NULL}
};

// DEMONIC STONE (FAREWELL FOREST)
DialogEntry dialDemStoneWithoutKey[] = {
	{"MOBBOB", "Bob", "Hey, where's my Spirit Key ?!"},
	{NULL, NULL, NULL}
};

void DemStoneGoto()
{
	MobState *state = (MobState*) GetGameData("MOBDEMSTONE1", sizeof(MobState));
	state->sceneID = -1;

	state = (MobState*) GetGameData("MOBTODD", sizeof(MobState));
	state->sceneID = Scene::ShadowRealm;
	state->x = 5;
	state->y = 5;

	sceneView.setScene(Scene::ShadowRealm);
};

DialogEntry dialDemStone_goto[] = {
	{"MOBBOB", "Bob", "OK, here we go!", DemStoneGoto},
	{"MOBTODD", "Todd", "WOW! This place is... uh..."},
	{"MOBCASPAR", "Caspar", "Oh my God!"},
	{"MOBBOB", "Bob", "Yes, I know, it can be quite shocking to those who have never seen it before."},
	{"MOBBOB", "Bob", "Now we have to get to the Shadow Dungeon. That's where they probably took the Chief. But we must watch out for the strange monsters and lost souls around here!"},
	{NULL, NULL, NULL}
};

DialogEntry dialDemStone[] = {
	{"MOBTODD", "Todd", "Is this the demonic stone?"},
	{"MOBBOB", "Bob", "Yes. Allow me to use my Spirit Key to access the Shadow Realm."},
	{"MOBBOB", "Bob", "Are you ready for it?"},
	{"MOBTODD", "Todd", NULL, NULL, 2, {
		{"Yes, let's go!", dialDemStone_goto},
		{"No, not yet.", &dialEmpty}
	}},
	{NULL, NULL, NULL}
};

// SHADOW PRIEST
void ShadowPriestBattle()
{
	MobState *state = (MobState*) GetGameData("MOBSHPRIEST", sizeof(MobState));
	state->sceneID = -1;

	StartBattle(new EnemyShadowPriest, new EnemyShadowWarrior, new EnemyShadowWarrior, new EnemyShadowWarrior);
	battleView.setFlee(false);
};

DialogEntry dialShadowPriest[] = {
	{"MOBSHPRIEST", "???", "Who are you?! How dare you try to enter the Shadow Temple?!"},
	{"MOBCASPAR", "Caspar", "We have come to look for the Chief of Eastville, who we believe may be in this temple."},
	{"MOBSHPRIEST", "???", "Ah... the prisoner. The one who wanted to tell the secret..."},
	{"MOBTODD", "Todd", "What secret? And why did you take him as a prisoner? And who are you?!"},
	{"MOBSHPRIEST", "Shadow Priest", "I am the Shadow Priest. Your Chief tried exposing our secret. Now that you know about it, you have to die.", ShadowPriestBattle},
	{NULL, NULL, NULL}
};


// SHADOW NECROMANCER
void ShadowNecroBattle()
{
	MobState *state = (MobState*) GetGameData("MOBSHNECRO", sizeof(MobState));
	state->sceneID = -1;

	StartBattle(new EnemyShadowNecromancer);
	battleView.setFlee(false);
};

DialogEntry dialShadowNecromancer[] = {
	{"MOBSHNECRO", "Shadow Necromancer", "STOP!", ShadowNecroBattle},
	{NULL, NULL, NULL}
};

