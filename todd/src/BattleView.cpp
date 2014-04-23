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

using namespace std;

BattleView battleView;

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
			else if ((ev->key.keysym.sym == SDLK_x) || (ev->key.keysym.sym == SDLK_RIGHT))
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
				else if (optionSel == 3) // Wait
				{
					optionSel = 0;
					schedTurn();
				}
				else if (optionSel == 4) // Flee
				{
					if (canFlee)
					{
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
					if (targetSel != 3) targetSel++;
					string name = GetPartyMember(targetSel);
					if (name == "")
					{
						targetSel--;
					}
					else
					{
						Character *chr = GetChar(name);
						if ((chr->getHP() == 0) && (!skillSel->isUsableAgainstDead())) targetSel--;
					};
				}
				else if ((targetSel > 4) && (targetSel < 8))
				{
					int sel = targetSel - 4;
					if (sel != 3) sel++;

					Enemy *enemy = enemies[sel];
					if (enemy == NULL)
					{
						sel--;
					}
					else
					{
						if ((enemy->hp == 0) && (!skillSel->isUsableAgainstDead()))
						{
							sel--;
						};
					};

					targetSel = sel + 4;
				};
			}
			else if (ev->key.keysym.sym == SDLK_UP)
			{
				if (targetSel < 4)
				{
					if (targetSel != 0) targetSel--;
					string name = GetPartyMember(targetSel);
					if (name == "")
					{
						targetSel++;
					}
					else
					{
						Character *chr = GetChar(name);
						if ((chr->getHP() == 0) && (!skillSel->isUsableAgainstDead())) targetSel++;
					};
				}
				else if ((targetSel > 4) && (targetSel < 8))
				{
					int sel = targetSel - 4;
					if (sel != 0) sel--;

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
						};
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
			else if (ev->key.keysym.sym == SDLK_z)
			{
				mode = Mode::MENU;
			}
			else if (ev->key.keysym.sym == SDLK_x)
			{
				skillSel->init(targetSel);
				mode = Mode::SKILL;
			};
		};
	}
	else if (mode == Mode::SKILL_SELECT)
	{
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == SDLK_z)
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
			else if (ev->key.keysym.sym == SDLK_x)
			{
				Skillset *skillset = GetSkillset(GetPartyMember(turn));
				skillSel = skillset->skills[skillSelIndex];
				mode = Mode::TARGET;
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
			plotX += 276;
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
			plotX += 276;
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
		ssSkillMenu->draw(660, 0, 0, false);
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
			text.draw(710, plotY);

			plotY += 24;
		};
	}
	else if (mode == Mode::VICTORY)
	{
		Text text("VICTORY!", 255, 255, 0, 255, fntCaption);
		text.draw(480, 240, Text::CENTER, Text::CENTER);
	};

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
	do
	{
		turn++;
		if (turn == 8) turn = 0;
	} while (!canMove(turn));

	if (turn < 4)
	{
		mode = Mode::MENU;
	}
	else
	{
		Enemy *enemy = enemies[turn-4];
		skillSel = enemy->plan();
		mode = Mode::SKILL;
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
	else
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
				int xp = damage * 0.5;
				if (i == turn) xp = damage * 0.75;
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

void StartBattle(Enemy *a, Enemy *b, Enemy *c, Enemy *d)
{
	currentView = &battleView;
	battleView.init(a, b, c, d);
};
