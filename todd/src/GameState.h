/*
===============================================================================================

	Todd RPG

	Copyright (c) 2014-2016, Madd Games.
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
 * GameState.h
 * Managing the game state.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <stdlib.h>
#include <inttypes.h>

using namespace std;

/**
 * This 8-byte header is located at offset 0 in every .sav file.
 */
struct SaveHeader
{
	uint8_t			magic[4];		/* "TODD" */
	uint32_t		count;			/* number of state segments */
	uint8_t			name[32];		/* assigned by the player */
};

string GetPathToSaveSlot(int slot);
void NewGame();
void LoadGame(int slot);
void SaveGame(int slot, string name);

/**
 * \brief Access to game data.
 *
 * This function will find a game state segment with a specific name in the current
 * game state, and return a pointer to it if it exists. If the segment is too short,
 * it shall be expanded. If it doesn't exist, it will be created. If it's too long,
 * then the rest of it shall be ignored. This function will return a pointer to that segment,
 * buffered in RAM - that pointer may become invalid upon the next call to GetGameData().
 *
 * Any data that was not yet accessed (as a result of expansion or creation) is initialised to 0.
 *
 * \param name Names the segment.
 * \param size The expected size of the segment.
 * \return A pointer to the game state segment buffer.
 */
void *GetGameData(string name, size_t size);
void RemoveGameData(string name);
void PrintGameData();

#endif
