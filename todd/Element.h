/**
 * Element.h
 * Enumerations and various procedures related to elements.
 */

#ifndef ELEMENT_H
#define ELEMENT_H

class Element
{
public:
	enum
	{
		/**
		 * Natural elements - fire beats air, air beats earth, earth beats water, water beats fire.
		 */
		FIRE = 0,
		WATER = 1,
		AIR = 2,
		EARTH = 3,

		/**
		 * Moral elements - light and darkness beat each other.
		 */
		LIGHT = 4,
		DARKNESS = 5,

		/**
		 * Special elements - neutral is not good or bad against anything, "divine" beats itself only.
		 */
		NEUTRAL = 6,
		DIVINE = 7,

		NUM_ELEMENTS = 8
	};
};

#endif
