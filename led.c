// led.c
#include "led.h"
#include "consoleUtils.h"
#include <stdbool.h>
/******************************************************************************
 **              Variables
 ******************************************************************************/
static volatile _Bool s_isTimeToChangeSpeed = false;
static const char *s_speed = 0;


/******************************************************************************
 **              Public functions
 ******************************************************************************/
void Led_init(void)
{
	// Often would have something to do here in other modules,
	// so included here as just a sample.
}

void Led_setSpeed(const char *newSpeed)
{
	s_speed = newSpeed;
}

void Led_notifyOnTimeIsr(void)
{
	s_isTimeToChangeSpeed = true;
}

void Led_doBackgroundWork(void)
{
	if (s_isTimeToChangeSpeed) {
		s_isTimeToChangeSpeed = false;

		// Do we have a message, and not at its end?
		if (s_speed && *s_speed) {
			ConsoleUtilsPutChar(*s_speed);
			s_speed++;
		}
	}
}

