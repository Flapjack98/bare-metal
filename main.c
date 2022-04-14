#include "consoleUtils.h"

// My hardware abstraction modules
#include "joystick.h"
#include "serial.h"
#include "timer.h"
#include "wdtimer.h"

// My application's modules
#include "joystickHandler.h"
#include "ledFlasher.h"
#include "serialHandler.h"
// #include "resetSource.h"

/******************************************************************************
 **              INTERNAL CONSTANT DEFINITIONS
 ******************************************************************************/
static const unsigned int TIMER_PERIOD_MILLISECONDS = 10;

int main(void)
{

	// Initialization
	Serial_init();
	Joystick_init();
	LedFlasher_init();
	Watchdog_init();
	Timer_init(TIMER_PERIOD_MILLISECONDS);

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(SerialHandler_notifyOnSerialRxIsr);
	Timer_setTimerIsrCallback(LedFlasher_notifyOnTimeIsr);

	// Display welcome message
	ConsoleUtilsPrintf("\nLightBouncer:\n");
	ConsoleUtilsPrintf("   by Catherine and Noel\n");
	ConsoleUtilsPrintf("------------------------\n");
	
	// Display reset sources
	// ResetSource_displayResetSource();

	// Main loop:
	while(1) {
		// Handle background processing
		SerialHandler_doBackgroundWork();
		LedFlasher_doBackgroundWork();
		JoystickHandler_doBackgroundWork();

		// Timer ISR signals intermittent background activity.
		if(Timer_isIsrFlagSet()) {
			Timer_clearIsrFlag();

			// Hit watchdog every timer period (10ms) and if 'x' wasn't typed.
			if (Watchdog_shouldHit()) {
				Watchdog_hit();
			}
		}
	}
}
