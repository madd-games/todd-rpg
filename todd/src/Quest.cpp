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
 * Quest.cpp
 * Describes quests.
 */

#include "Quest.h"
#include "GameState.h"

Quest quests[] = {
	Quest("QSTCHEASTV", "Chief of Eastville", "The Chief of Eastville has been kidnapped. You must go to Eastville and speak to the locals to help track down the kidnappers."),

	// END
	Quest("", "", "")
};

Quest::Quest(string id, string title, string desc) : id(id), title(title), desc(desc)
{
};

bool Quest::isID(string id)
{
	return this->id == id;
};

Quest::Status Quest::getStatus()
{
	Quest::Status *stptr = (Quest::Status*) GetGameData(id, sizeof(Quest::Status));
	return *stptr;
};

void Quest::setStatus(Quest::Status status)
{
	Quest::Status *stptr = (Quest::Status*) GetGameData(id, sizeof(Quest::Status));
	*stptr = status;
};

string Quest::getTitle()
{
	return title;
};

string Quest::getDescription()
{
	return desc;
};

Quest* GetQuest(string id)
{
	Quest *quest = quests;
	while (!quest->isID(""))
	{
		if (quest->isID(id))
		{
			return quest;
		};

		quest++;
	};
	return NULL;
};

Quest *GetQuestList()
{
	return quests;
};
