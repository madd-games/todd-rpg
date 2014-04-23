/**
 * Timer.h
 * A timer that guarantees that all events in the game will be synchornised.
 */

#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
	static unsigned long state;

public:
	static void Update();
	static unsigned long Read();
};

#endif
