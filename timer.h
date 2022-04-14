/* This Timer module is responsible for creating a bare metal timer using
 * interrupts for its implementation.
 * The interval may be specified in milliseconds.
 * NOTE: This module assumes that the timer being used is
 * 32-bits in size and has a 24MHz clock. */

#ifndef TIMER_GAURD_H
#define TIMER_GAURD_H

/* timerIntervalMilliseconds: Time before the next timer interrupt occurs.
 * The timer will occur repeatedly every timerIntervalMilliseconds.*/
void Timer_init(unsigned int timerIntervalMilliseconds);

// Disables the timer.
void Timer_cleanup(void);

// Checks if the isrFlag has been set.
// In other words: return true if the timer has occurred, false if not yet.
_Bool Timer_isIsrFlagSet(void);

// Clear the timer flag to wait for another timer to occur and detect it.
void Timer_clearIsrFlag(void);

// Set the callback function for the timer interrupt service routine.
void Timer_setTimerIsrCallback(void (*isrCallback)(void));

#endif
