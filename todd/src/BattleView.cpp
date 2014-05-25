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
 * BattleView.cpp
 * Displays battles.
 */

#include "BattleView.h"
#include "Todd.h"
#include "Character.h"
#include "Text.h"
#include <sstream>
#include "Timer.h"
#include "Skill.h"
#include <iostream>
#include "Skillset.h"
#include <stdlib.h>
#include "SceneView.h"
#include "LootView.h"
#include "Item.h"
#include <time.h>
#include "Options.h"

using namespace std;

BattleView battleView;

#define	NUM_EXPENDABLE_ITEMS		1
int expendableItems[NUM_EXPENDABLE_ITEMS] = {
	Item::POTION
};

Skill *itemSkills[NUM_EXPENDABLE_ITEMS] = {
	skillPotion
};

void BattleView::init(Enemy *a, Enemy *b, Enemy *c, Enemy *d)
{
	enemies.clear();
	enemies.push_back(a);
	enemies.push_back(b);
	enemies.push_back(c);
	enemies.push_back(d);

	turn = 0;
	mode = Mode::MENU;
	optionSel = 0;
	canFlee = true;

	dmgDisplays.clear();
	particles.clear();
	loots.clear();
	if (a != NULL) a->dropItems(loots);
	if (b != NULL) b->dropItems(loots);
	if (c != NULL) c->dropItems(loots);
	if (d != NULL) d->dropItems(loots);
};

void BattleView::handleEvent(SDL_Event *ev)
{
	if (mode == Mode::MENU)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == SDLK_DOWN)
			{
				if (optionSel != 4) optionSel++;
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (optionSel != 0) optionSel--;
			}
			else if ((ev->key.keysym.sym == options.confirmKey) || (ev->key.keysym.sym == SDLK_RIGHT))
			{
				if (optionSel == 0)			// Attack
				{
					skillSel = skillAttack;
					mode = Mode::TARGET;

					if (!skillSel->isOffensive())
					{
						targetSel = turn;
					}
					else
					{
						int sel = 0;
						while (1)
						{
							Enemy *enemy = enemies[sel];
							if (enemy == NULL)
							{
								sel++;
							}
							else
							{
								if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
								{
									sel++;
								}
								else
								{
									break;
								};
							};
						};

						targetSel = sel + 4;
					};
				}
				else if (optionSel == 1) // Skills
				{
					skillSelIndex = 0;
					mode = Mode::SKILL_SELECT;
				}
				else if (optionSel == 2) // Items
				{
					itemSelIndex = 0;
					mode = Mode::ITEM_SELECT;
				}
				else if (optionSel == 3) // Wait
				{
					optionSel = 0;
					schedTurn();
				}
				else if (optionSel == 4) // Flee
				{
					if (canFlee)
					{
						int i;
						for (i=0; i<4; i++)
						{
							Enemy *enemy = enemies[i];
							if (enemy != NULL) delete enemy;
						};
						currentView = &sceneView;
					};
				};
			};
		};
	}
	else if (mode == Mode::TARGET)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == SDLK_DOWN)
			{
				if (targetSel < 4)
				{
					int orgSel = targetSel;
					while (1)
					{
						targetSel++;
						if (targetSel == 4)
						{
							targetSel = orgSel;
							break;
						};

						string name = GetPartyMember(targetSel);
						if (name == "")
						{
							continue;
						}
						else
						{
							Character *chr = GetChar(name);
							if ((chr->getHP() == 0) && (!skillSel->isUsableAgainstDead())) continue;
						};

						break;
					};
				}
				else if ((targetSel >= 4) && (targetSel < 8))
				{
					int sel = targetSel - 4;
					int orgSel = sel;

					while (1)
					{
						sel++;
						if (sel == 4)
						{
							sel = orgSel;
							break;
						};

						Enemy *enemy = enemies[sel];
						if (enemy == NULL)
						{
							continue;
						}
						else
						{
							if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
							{
								continue;
							};
						};

						break;
					};

					targetSel = sel + 4;
				};
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (targetSel < 4)
				{
					int orgSel = targetSel;
					while (1)
					{
						targetSel--;
						if (targetSel == -1)
						{
							targetSel = orgSel;
							break;
						};

						string name = GetPartyMember(targetSel);
						if (name == "")
						{
							continue;
						}
						else
						{
							Character *chr = GetChar(name);
							if ((chr->getHP() == 0) && (!skillSel->isUsableAgainstDead())) continue;
						};

						break;
					};
				}
				else if ((targetSel >= 4) && (targetSel < 8))
				{
					int sel = targetSel - 4;
					int orgSel = sel;

					while (1)
					{
						sel--;
						if (sel == -1)
						{
							sel = orgSel;
							break;
						};

						Enemy *enemy = enemies[sel];
						if (enemy == NULL)
						{
							continue;
						}
						else
						{
							if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
							{
								continue;
							};
						};

						break;
					};

					targetSel = sel + 4;
				};
			}
			else if (ev->key.keysym.sym == SDLK_RIGHT)
			{
				if (targetSel < 4)
				{
					int sel = 0;
					while (1)
					{
						Enemy *enemy = enemies[sel];
						if (enemy == NULL)
						{
							sel++;
						}
						else
						{
							if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
							{
								sel++;
							}
							else
							{
								break;
							};
						};
					};
					targetSel = sel + 4;
				};
			}
			else if (ev->key.keysym.sym == SDLK_LEFT)
			{
				if (targetSel >= 4)
				{
					targetSel = 0;
					while (1)
					{
						string name = GetPartyMember(targetSel);
						if (name == "")
						{
							targetSel++;
						}
						else
						{
							Character *chr = GetChar(name);
							if ((chr->getHP() == 0) && (!skillSel->isUsableAgainstDead()))
							{
								targetSel++;
							}
							else
							{
								break;
							};
						};
					};
				};
			}
			else if (ev->key.keysym.sym == options.cancelKey)
			{
				mode = Mode::MENU;
			}
			else if (ev->key.keysym.sym == options.confirmKey)
			{
				Character *chr = GetChar(GetPartyMember(turn));
				int mana = chr->getMP() - skillSel->getManaUse();
				if (mana < 0) mana = 0;
				chr->setMP(mana);

				skillSel->init(targetSel);
				mode = Mode::SKILL;
			};
		};
	}
	else if (mode == Mode::SKILL_SELECT)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == options.cancelKey)
			{
				mode = Mode::MENU;
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (skillSelIndex != 0) skillSelIndex--;
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				Skillset *skillset = GetSkillset(GetPartyMember(turn));
				int limit = (int) skillset->skills.size() - 1;
				if (skillSelIndex != limit) skillSelIndex++;
			}
			else if ((ev->key.keysym.sym == options.confirmKey) || (ev->key.keysym.sym == SDLK_RIGHT))
			{
				Skillset *skillset = GetSkillset(GetPartyMember(turn));
				skillSel = skillset->skills[skillSelIndex];
				Character *chr = GetChar(GetPartyMember(turn));
				if (chr->getMP() < skillSel->getManaUse())
				{
					return;
				};
				mode = Mode::TARGET;

				if (!skillSel->isOffensive())
				{
					targetSel = turn;
				}
				else
				{
					int sel = 0;
					while (1)
					{
						Enemy *enemy = enemies[sel];
						if (enemy == NULL)
						{
							sel++;
						}
						else
						{
							if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
							{
								sel++;
							}
							else
							{
								break;
							};
						};
					};

					targetSel = sel + 4;
				};
			};
		};
	}
	else if (mode == Mode::ITEM_SELECT)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == options.cancelKey)
			{
				mode = Mode::MENU;
			}
			else if ((ev->key.keysym.sym == options.confirmKey) || (ev->key.keysym.sym == SDLK_RIGHT))
			{
				int id = expendableItems[itemSelIndex];
				Character *chr = GetChar(GetPartyMember(turn));
				Container *cont = chr->getInventory();
				if (cont->count(id) == 0)
				{
					return;
				};

				skillSel = itemSkills[itemSelIndex];
				mode = Mode::TARGET;
				if (!skillSel->isOffensive())
				{
					targetSel = turn;
				}
				else
				{
					int sel = 0;
					while (1)
					{
						Enemy *enemy = enemies[sel];
						if (enemy == NULL)
						{
							sel++;
						}
						else
						{
							if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
							{
								sel++;
							}
							else
							{
								break;
							};
						};
					};

					targetSel = sel + 4;
				};
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (itemSelIndex != 0) itemSelIndex--;
			}
			else if (ev->key.keysym.sym == SDLK_DOWN)
			{
				if (itemSelIndex != (NUM_EXPENDABLE_ITEMS-1)) itemSelIndex++;
			};
		};
	};
};

void BattleView::render()
{
	// Check if this is victory.
	bool anyAlive = false;
	int i;
	for (i=0; i<4; i++)
	{
		Enemy *enemy = enemies[i];
		if (enemy != NULL)
		{
			if (enemy->hp > 0)
			{
				anyAlive = true;
			};
		};
	};

	if ((!anyAlive) && (mode == Mode::MENU))
	{
		mode = Mode::VICTORY;
		victoryTimer = Timer::Read();
	};

	// Render the background
	SpriteSheet *bg = getBackground(sceneView.getScene());
	if (bg != NULL)
	{
		bg->draw(0, 0, 0, false);
	};

	// Render the allies.
	int plotY = 100;
	int plotX = 2;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name == "") break;
		else
		{
			Character *chr = GetChar(name);
			if (chr->getHP() != 0) chr->getSpriteSheet()->draw(348, plotY, 6, false);
			plotY += 50;

			int y = 380;
			stringstream ss;
			ss << "Level " << chr->getLevel();

			Text txt1(chr->getName(), 255, 255, 255);
			Text txt2(ss.str(), 0, 0, 255);

			txt1.draw(plotX+41, y+50);
			txt2.draw(plotX+15, y+65);

			DrawBar(plotX+100, y+10, chr->getHP(), chr->getMaxHP(), 0);
			DrawBar(plotX+100, y+30, chr->getMP(), chr->getMaxMP(), 1);
			DrawBar(plotX+100, y+50, chr->getXP(), chr->getMaxXP(), 2);

			chr->getSpriteSheet()->draw(plotX+10, y+5, 0, false);
			ssElements->draw(plotX+15, y+50, chr->getElement(), false);
			plotX += 241;
		};
	};

	// Render the enemies.
	plotY = 100;
	plotX = 2;
	for (i=0; i<4; i++)
	{
		Enemy *enemy = enemies[i];
		if (enemy != NULL)
		{
			if (enemy->hp != 0) enemy->spriteSheet->draw(500, plotY, 3, false);
			plotY += 50;

			int y = 2;
			stringstream ss;
			ss << "Level " << enemy->level;

			Text txt1(enemy->name, 255, 255, 255);
			Text txt2(ss.str(), 0, 0, 255);

			txt1.draw(plotX+41, y+50);
			txt2.draw(plotX+15, y+65);

			DrawBar(plotX+100, y+10, enemy->hp, enemy->maxhp, 0);

			enemy->spriteSheet->draw(plotX+10, y+5, 0, false);
			ssElements->draw(plotX+15, y+50, enemy->element, false);
			plotX += 241;
		};
	};

	if (mode == Mode::MENU)
	{
		ssBattleMenu->draw(700, 154, 0, false);
		Character *chr = GetChar(GetPartyMember(turn));
		chr->getSpriteSheet()->draw(702, 156, 0, false);

		Text text(chr->getName(), 255, 255, 255, 255, fntBattleCaption);
		text.draw(750, 180, Text::LEFT, Text::CENTER);

		Skillset *skillset = GetSkillset(GetPartyMember(turn));

		vector<string> options;
		options.push_back("Attack");
		options.push_back(skillset->name);
		options.push_back("Items");
		options.push_back("Wait");
		options.push_back("Flee");

		int i;
		plotY = 210;
		for (i=0; i<5; i++)
		{
			int red=255, green=255, blue=255;
			if (optionSel == i)
			{
				red = 0;
				green = 127;
			};
			if ((i == 4) && (!canFlee))
			{
				red = green = blue = 0;
			};
			Text txt(options[i], red, green, blue, 255, fntText);
			txt.draw(726, plotY);
			plotY += 24;
		};

		int xpos = 702;
		if ((Timer::Read() % 1000) < 500) xpos += 2;
		ssCursor->draw(xpos, 208+(optionSel*24), 0, false);
	}
	else if (mode == Mode::TARGET)
	{
		if (targetSel < 4)
		{
			int x = 340;
			if ((Timer::Read() % 1000) < 500) x += 2;
			ssCursor->draw(x, 112+50*targetSel, 0, false);
		}
		else
		{
			int x = 492;
			if ((Timer::Read() % 1000) < 500) x += 2;
			ssCursor->draw(x, 112+50*(targetSel-4), 0, false);
		};
	}
	else if (mode == Mode::SKILL)
	{
		ssElements->draw(850, 20, skillSel->getElement(), false);
		Text text(skillSel->getName(), 255, 255, 255);
		text.draw(874, 20);

		skillSel->act();
		if (!skillSel->isActive())
		{
			schedTurn();
		};
	}
	else if (mode == Mode::SKILL_SELECT)
	{
		ssSkillMenu->draw(360, 0, 0, false);
		Skillset *skillset = GetSkillset(GetPartyMember(turn));
		int i;
		int plotY = 2;
		for (i=0; i<(int)skillset->skills.size(); i++)
		{
			int red=255, green=255, blue=255;
			if (skillSelIndex == i)
			{
				int x = 662;
				if ((Timer::Read() % 1000) < 500) x += 2;
				ssCursor->draw(x, plotY, 0, false);

				blue = 255;
				green = 127;
				red = 0;
			};

			Skill *skill = skillset->skills[i];
			ssElements->draw(686, plotY, skill->getElement(), false);
			Text text(skill->getName(), red, green, blue, 255);
			text.draw(710, plotY+2);

			if (i == skillSelIndex)
			{
				Text desc(skill->getDesc(), 255, 255, 255, 255, fntText, 295);
				desc.draw(362, 2);
			};

			stringstream ss;
			ss << skillset->skills[i]->getManaUse();
			Text txtMana(ss.str(), red, green, blue, 255);
			txtMana.draw(958, plotY+2, Text::RIGHT);

			plotY += 24;
		};
	}
	else if (mode == Mode::ITEM_SELECT)
	{
		ssSkillMenu->draw(360, 0, 0, false);
		int plotY = 2;
		int i;
		for (i=0; i<NUM_EXPENDABLE_ITEMS; i++)
		{
			int red=255, green=255, blue=255;
			if (itemSelIndex == i)
			{
				int x = 662;
				if ((Timer::Read() % 1000) < 500) x += 2;
				ssCursor->draw(x, plotY, 0, false);

				blue = 255;
				green = 127;
				red = 0;
			};

			Character *chr = GetChar(GetPartyMember(turn));
			int count = chr->getInventory()->count(expendableItems[i]);
			if (count > 0)
			{
				Item *item = GetItem(expendableItems[i]);
				ssItems->draw(686, plotY, expendableItems[i], false);

				Text text(item->getName(), red, green, blue, 255);
				text.draw(710, plotY+2);

				stringstream ss;
				ss << count;
				Text txtCount(ss.str(), red, green, blue);
				txtCount.draw(958, plotY+2, Text::RIGHT);

				if (i == itemSelIndex)
				{
					Text desc(item->getDesc(), 255, 255, 255, 255, fntText, 295);
					desc.draw(362, 2);
				};
			};

			plotY += 24;
		};
	}
	else if (mode == Mode::VICTORY)
	{
		Text text("VICTORY!", 255, 255, 0, 255, fntCaption);
		text.draw(480, 240, Text::CENTER, Text::CENTER);

		if ((Timer::Read() - victoryTimer) >= 2000)
		{
			int i;
			for (i=0; i<4; i++)
			{
				Enemy *enemy = enemies[i];
				if (enemy != NULL) delete enemy;
			};

			lootView.init(loots);
			currentView = &lootView;
		};
	}
	else if (mode == Mode::GAME_OVER)
	{
		Text text("GAME OVER", 255, 0, 0, 255, fntCaption);
		text.draw(480, 240, Text::CENTER, Text::CENTER);
	};

	// Render particles.
	vector<Particle>::iterator pit = particles.begin();
	while (pit != particles.end())
	{
		int stage = (int)((Timer::Read() - pit->timeStart) / pit->timeStage);
		int delta = (int)(Timer::Read() - pit->timeStart);

		if (stage >= pit->numStages)
		{
			pit = particles.erase(pit);
		}
		else
		{
			pit->ss->draw(pit->x+pit->vx*delta, pit->y+pit->vy*delta, stage, false);
			pit++;
		};
	};

	// Render damage displays.
	vector<DamageDisplay>::iterator it = dmgDisplays.begin();
	while (it != dmgDisplays.end())
	{
		int offset = (int) (Timer::Read()-it->start) / 10;
		int x = it->x;
		int y = it->y;
		if (offset < 24)
		{
			y -= offset;
		}
		else
		{
			y -= 24;
			y += (offset - 24);
		};

		ssElements->draw(x, y, it->element, false);
		Text text(it->value, it->red, it->green, it->blue, 255);
		text.draw(x+24, y);

		if (offset >= 48)
		{
			it = dmgDisplays.erase(it);
		}
		else
		{
			it++;
		};
	};
};

bool BattleView::canMove(int entity)
{
	if (entity < 4)
	{
		string name = GetPartyMember(entity);
		if (name == "") return false;

		Character *chr = GetChar(name);
		if (chr->getHP() == 0) return false;
	}
	else
	{
		int index = entity - 4;
		Enemy *enemy = enemies[index];
		if (enemy == NULL) return false;
		if (enemy->hp == 0) return false;
	};

	return true;
};

void BattleView::schedTurn()
{
	// Check if this is a failure.
	bool anyAlive = false;
	int i;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			if (chr->getHP() > 0)
			{
				anyAlive = true;
			};
		};
	};

	if (!anyAlive)
	{
		mode = Mode::GAME_OVER;
		return;
	};

	do
	{
		turn++;
		if (turn == 8) turn = 0;
	} while (!canMove(turn));

	if (turn < 4)
	{
		optionSel = 0;
		mode = Mode::MENU;
	}
	else
	{
		Enemy *enemy = enemies[turn-4];
		skillSel = enemy->plan();
		mode = Mode::SKILL;
	};
};

SpriteSheet *BattleView::getBackground(int sceneID)
{
	switch (sceneID)
	{
	case Scene::Forest:
		return ssForestBackground;
	default:
		return NULL;
	};
};

CharStats BattleView::getStats(int entity)
{
	if (entity < 4)
	{
		Character *chr = GetChar(GetPartyMember(entity));
		CharStats stats = *chr->getStats();
		Container *cont = chr->getInventory();

		int i;
		for (i=0; i<7; i++)
		{
			CharStats tst;
			memset(&tst, 0, sizeof(CharStats));

			int id = cont->get(i).id;
			if (id != 0)
			{
				Item *item = GetItem(id);
				item->getStat(tst);

				stats.STR += tst.STR;
				stats.INT += tst.INT;
				stats.DEF += tst.DEF;
				stats.MDEF += tst.MDEF;
			};
		};

		return stats;
	}
	else
	{
		return enemies[entity-4]->stats;
	};
};

int* BattleView::getResistances(int entity)
{
	if (entity < 4)
	{
		Character *chr = GetChar(GetPartyMember(entity));
		return chr->getResistances();
	}
	else
	{
		return enemies[entity-4]->resist;
	};
};

int BattleView::getLevel(int entity)
{
	if (entity < 4)
	{
		Character *chr = GetChar(GetPartyMember(entity));
		return chr->getLevel();
	}
	else
	{
		return enemies[entity-4]->level;
	};
};

int BattleView::getTurn()
{
	return turn;
};

void BattleView::attack(int target, int type, int element, int damage)
{
	CharStats ustats = getStats(turn);
	CharStats tstats = getStats(target);
	int *resist = getResistances(target);

	if (type == AttackType::PHYSICAL)
	{
		damage += damage * ustats.STR / 100;
		damage -= damage * tstats.DEF / 100;
	}
	else if (type == AttackType::MAGIC)
	{
		damage += damage * ustats.INT / 100;
		damage -= damage * tstats.MDEF / 100;
	};

	damage -= (damage * resist[element] / 100);

	if (target < 4)
	{
		Character *chr = GetChar(GetPartyMember(target));
		chr->dealDirectDamage(damage);
	}
	else
	{
		Enemy *enemy = enemies[target-4];
		enemy->hp -= damage;
		if (enemy->hp < 0) enemy->hp = 0;
		if (enemy->hp > enemy->maxhp) enemy->hp = enemy->maxhp;
	};

	if (turn < 4)
	{
		int i;
		for (i=0; i<4; i++)
		{
			string name = GetPartyMember(i);
			if (name != "")
			{
				Character *chr = GetChar(name);
				int xp = 5 * chr->getLevel();
				if (i == turn) xp *= 2;
				if (xp < 0) xp = -xp;
				chr->train(xp);
			};
		};
	};

	DamageDisplay disp;
	disp.element = element;
	if (damage < 0)
	{
		disp.red = 0;
		disp.green = 255;
		disp.blue = 0;
		damage *= -1;
	}
	else
	{
		disp.red = 255;
		disp.green = 255;
		disp.blue = 255;
	};
	stringstream ss;
	ss << damage;
	disp.value = ss.str();
	disp.start = Timer::Read();

	if (target < 4)
	{
		disp.x = 396;
		disp.y = 100+50*target;
	}
	else
	{
		disp.x = 548;
		disp.y = 100+50*(target-4);
	};
	dmgDisplays.push_back(disp);
};

int BattleView::getRandomAlly(bool allowDead)
{
	vector<int> chooseable;
	int i;
	for (i=0; i<4; i++)
	{
		string name = GetPartyMember(i);
		if (name != "")
		{
			Character *chr = GetChar(name);
			if ((chr->getHP() != 0) || (allowDead))
			{
				chooseable.push_back(i);
			};
		};
	};

	srand(time(NULL));
	return chooseable[rand() % chooseable.size()];
};

void BattleView::setFlee(bool flee)
{
	canFlee = flee;
};

void BattleView::emitParticle(int entity, int offX, int offY, int type)
{
	int x, y;
	if (entity < 4)
	{
		x = 348;
		y = 100 + 50 * entity;
	}
	else
	{
		x = 500;
		y = 100 + 50 * (entity-4);
	};

	Particle part;
	part.x = x + offX;
	part.y = y + offY;
	part.timeStart = Timer::Read();
	part.vx = 0;
	part.vy = 0;

	switch (type)
	{
	case SPARK:
		part.ss = ssSpark;
		part.timeStage = 100;
		part.numStages = 5;
		part.x -= 8;
		part.y -= 8;
		break;
	case FLAME:
		part.ss = ssFlame;
		part.timeStage = 250;
		part.numStages = 5;
		part.x -= 8;
		part.y -= 8;
		part.vy = -(1.0/20.0);
		break;
	};

	particles.push_back(part);
};

void StartBattle(Enemy *a, Enemy *b, Enemy *c, Enemy *d)
{
	currentView = &battleView;
	battleView.init(a, b, c, d);
};
