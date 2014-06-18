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
 * Quest.h
 * Describes quests.
 */

#ifndef QUEST_H
#define QUEST_H

#include <string>

using namespace std;

/**
 * \brief Describes a quest.
 */
class Quest
{
private:
	string id, title, desc;

public:
	/**
	 * \brief Quest statuses.
	 */
	enum Status
	{
		/**
		 * The player has not found out about this quest yet.
		 */
		Hidden = 0,

		/**
		 * The player has found out about this quest, but not completed it.
		 */
		Active = 1,

		/**
		 * The player has completed the quest.
		 */
		Complete = 2,
	};

	/**
	 * \brief Constructor.
	 * \param id The quest ID - at most 16 characters, must be unique.
	 * \param title The quest title - this is visible to the player.
	 * \param desc Description of the quest (i.e. what has to be done). This is visible to the player.
	 */
	Quest(string id, string title, string desc);

	/**
	 * \brief Check if this quest has the specified id.
	 */
	bool isID(string id);
};

/**
 * \brief Return the quest identified by ID.
 * \return Pointer to a Quest instance on success, NULL if such quest doesn't exist.
 */
Quest *GetQuest(string id);

#endif
