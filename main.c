// Fake Typing bare metal sample application
// On the serial port, fakes

#include "consoleUtils.h"
#include <stdint.h>

// My hardware abstraction modules
#include "serial.h"
#include "timer.h"
#include "wdtimer.h"

// My application's modules
#include "joystick.h"
#include "led.h"

/******************************************************************************
 **              INTERNAL CONSTANT DEFINITIONS
 ******************************************************************************/
static const int TIMER_PERIOD_MILLISECONDS = 10;

/******************************************************************************
 **              GLOBAL VARIABLE DEFINITIONS
 ******************************************************************************/

/******************************************************************************
 **              Functions
 ******************************************************************************/

void displayHelpMenu()
{
    ConsoleUtilsPrintf(
		"\nCommands:\n \
		  ?   : Display this help message\n \
		  0-9 : Set speed 0 (slow) to 9 (fast)\n \
		  x   : Stop hitting the watchdog\n \
		  JOY : Up (faster), Down (slower)\n" \
	);
}



/******************************************************************************
 **              SERIAL PORT HANDLING
 ******************************************************************************/
static volatile uint8_t s_rxByte = 0;
static void serialRxIsrCallback(uint8_t rxByte) {
	s_rxByte = rxByte;
}

static void doBackgroundSerialWork(void)
{
	if (s_rxByte != 0) {
		if (s_rxByte == '?') {
			displayHelpMenu();
		} else if (s_rxByte >= '0' && s_rxByte <= '9'){
			Led_setSpeed(s_rxByte);
		} else if (s_rxByte == 'x') {
			Watchdog_stopHitting();
		} else {
			ConsoleUtilsPrintf("\nMust enter valid command\n");
			displayHelpMenu();
		}

		s_rxByte = 0;
	}
}

/******************************************************************************
 **              MAIN
 ******************************************************************************/
int main(void)
{

	// Initialization
	Serial_init(serialRxIsrCallback);
	Timer_init(TIMER_PERIOD_MILLISECONDS);
	Watchdog_init();

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(serialRxIsrCallback);
	Timer_setTimerIsrCallback(Led_notifyOnTimeIsr);

	// Display welcome message
	ConsoleUtilsPrintf("\nLightBouncer:\n");
	ConsoleUtilsPrintf("   by Catherine and Noel\n");
	ConsoleUtilsPrintf("------------------------\n");
	// Display reset sources
	//char *resetSource = readFromResetSource();

	// Main loop:
	while(1) {
		// Handle background processing
		doBackgroundSerialWork();
		Led_doBackgroundWork();
		Joystick_doBackgroundWork();

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
