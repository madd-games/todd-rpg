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
} __attribute__ ((packed));

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

#endif
