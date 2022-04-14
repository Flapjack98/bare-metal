/* The LED module Handles the LED logic for the LED flasher application,
 * by flashing the lights back at forth at different speeds, using the LED
 * hardware module to actually flash the led lights.*/

#include "hw_types.h"      // For _Bool
#include "led.h"
#include "ledFlasher.h"


/*****************************************************************************
 **                INTERNAL CONSTANTS
 *****************************************************************************/

static const unsigned int TIMER_PERIOD_MILLISECONDS = 10; // 10 ms
static const unsigned int DEFAULT_LED_SPEED = 0; // Slow speed by default
static const unsigned int DEFAULT_LIT_LED_POSITION = 0; // start at LED0

const unsigned int MAX_SPEED = 9;
const unsigned int MIN_SPEED = 0;

const unsigned int NUM_LEDS = 4;

static const Led_eLedNum LED_PINS[] = {
	LED0_PIN,
	LED1_PIN,
	LED2_PIN,
	LED3_PIN
};

/*****************************************************************************
 **                INTERNAL FUNCTION PROTOTYPES
 *****************************************************************************/
static unsigned int LedFlasher_calcFlashTimePeriod(unsigned int speed);
static unsigned int LedFlasher_calcPowerOfTwo(unsigned int exponent);

/*****************************************************************************
 **                INTERNAL STATIC VARIABLES
 *****************************************************************************/
static unsigned int s_speed;
static unsigned int s_litLedPosition;
static unsigned int s_flashTimePeriod;
static unsigned int s_curTimerPeriodCounter;
static _Bool s_areLedsMovingRight;
static volatile _Bool s_doBackgroundWork;

void LedFlasher_init(void)
{
	/* Set default LED speed */
	s_speed = DEFAULT_LED_SPEED;

	/* Set initial LED flash time period */
	s_flashTimePeriod = LedFlasher_calcFlashTimePeriod(s_speed);

	/* Set intial timer period counter */
	s_curTimerPeriodCounter = 0;

	/* Set initial flashing direction of LEDs */
	s_areLedsMovingRight = true;

	/* Initialize BBG leds */
	Led_init();

	/* Set initial LED lit position and flash it */
	s_litLedPosition = DEFAULT_LIT_LED_POSITION;
	Led_flashLed(LED_PINS[s_litLedPosition]);
}

void LedFlasher_notifyOnTimeIsr(void)
{
	s_doBackgroundWork = true;
}

void LedFlasher_doBackgroundWork(void)
{
	if (s_doBackgroundWork) {
		s_doBackgroundWork = false;
		
		++s_curTimerPeriodCounter;

		// If enough time has passed to flash the next LED position
		if (s_curTimerPeriodCounter * TIMER_PERIOD_MILLISECONDS
				>= s_flashTimePeriod)
		{
			s_curTimerPeriodCounter = 0;

			// Set the currently lit LED to off
			Led_unflashLed(LED_PINS[s_litLedPosition]);

			// Check edge case if lit led position is at end (LED3)
			if (s_litLedPosition == NUM_LEDS - 1) {
				s_areLedsMovingRight = false;
				s_litLedPosition = NUM_LEDS - 2;
			}
			// Check edge case if lit led position is at end (LED0)
			else if (s_litLedPosition == 0) {
				s_areLedsMovingRight = true;
				s_litLedPosition = 1;
			}
			// Check if lit led position is one of the middle two LEDs
			else {
				if (s_areLedsMovingRight) {
					++s_litLedPosition;
				} else {
					--s_litLedPosition;
				}
			}

			// Flash the next LED in the sequence
			Led_flashLed(LED_PINS[s_litLedPosition]);
		}
	}
}

void LedFlasher_setSpeed(unsigned int newSpeed)
{
	s_speed = newSpeed;
	s_flashTimePeriod = LedFlasher_calcFlashTimePeriod(s_speed);
}

void LedFlasher_decreaseSpeed(void)
{
	if (s_speed > MIN_SPEED) {
		LedFlasher_setSpeed(s_speed - 1);
	}
}

void LedFlasher_increaseSpeed(void)
{
	if (s_speed < MAX_SPEED) {
		LedFlasher_setSpeed(s_speed + 1);
	}
}

static unsigned int LedFlasher_calcFlashTimePeriod(unsigned int speed)
{
	return TIMER_PERIOD_MILLISECONDS * LedFlasher_calcPowerOfTwo(MAX_SPEED - speed);
}

static unsigned int LedFlasher_calcPowerOfTwo(unsigned int exponent)
{
	return 1 << exponent;
}