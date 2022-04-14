// joystick.c
#include "joystick.h"
#include "consoleUtils.h"
#include <stdbool.h>
/******************************************************************************
 **              Variables
 ******************************************************************************/
static volatile _Bool s_isTimeToChangeDirection = false;
static const char *s_direction = 0;


/******************************************************************************
 **              Public functions
 ******************************************************************************/
void Joystick_init(void)
{
	// Often would have something to do here in other modules,
	// so included here as just a sample.
}

void Joystick_setDirection(const char *newDirection)
{
	s_direction = newDirection;
}

void Joystick_notifyOnTimeIsr(void)
{
	s_isTimeToChangeDirection = true;
}

void Joystick_doBackgroundWork(void)
{
	if (s_isTimeToChangeDirection) {
		s_isTimeToChangeDirection = false;

		// Do we have a message, and not at its end?
		if (s_direction && *s_direction) {
			ConsoleUtilsPutChar(*s_direction);
			s_direction++;
		}
	}
}

