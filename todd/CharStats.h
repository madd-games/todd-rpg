/**
 * CharStats.h
 * The CharStats structure.
 */

#ifndef CHAR_STATS_H
#define CHAR_STATS_H

struct CharStats
{
	/**
	 * Strength.
	 * The percentage by which physical attacks are strengthened.
	 */
	int STR;

	/**
	 * Intellect.
	 * The percentage by which spells are strengthened.
	 */
	int INT;

	/**
	 * Defence (in %) against physical attacks.
	 */
	int DEF;

	/**
	 * Magic Defence (in %).
	 */
	int MDEF;
};

#endif
