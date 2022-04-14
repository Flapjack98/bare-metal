/* The LED module Handles the LED logic for the LED flasher application,
 * by flashing the lights back at forth at different speeds, using the LED
 * hardware API to flash the led lights.*/

#ifndef LED_FLASHER_H_
#define LED_FLASHER_H_

void LedFlasher_init(void);
void LedFlasher_setSpeed(unsigned int newSpeed);
void LedFlasher_notifyOnTimeIsr(void);
void LedFlasher_doBackgroundWork(void);

// Decrease speed by one, clamped to MIN_SPEED (0)
void LedFlasher_decreaseSpeed(void);

// Increase speed by one, clamped to MAX_SPEED (9)
void LedFlasher_increaseSpeed(void);

#endif
