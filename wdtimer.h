/* The wdtimer module provides an abstract interface to
 * implement watchdog functionality in a bare metal application.*/

#ifndef WDTIMER_H_
#define WDTIMER_H_

// Watchdog timer controls:
void Watchdog_init(void);
void Watchdog_hit(void);

// Returns true if the watchdog should still be hit, false if not.
_Bool Watchdog_shouldHit(void);

// Set an internal flag that makes Watchdog_shouldHit return false;
void Watchdog_stopHitting(void);

#endif /* WDTIMER_H_ */
