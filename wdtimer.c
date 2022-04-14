#include "dmtimer.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "error.h"
#include "watchdog.h"

#include "wdtimer.h"


/******************************************************************************
 **                      WATCHDOG FUNCTIONS
 *******************************************************************************/
#define WD_CLOCK          (32768L)
#define WD_TIMEOUT_S      (5)
#define WD_TIMEOUT_TICKS  (WD_TIMEOUT_S * WD_CLOCK)
#define WD_RESET_VALUE    ((unsigned int)0xFFFFFFFF - WD_TIMEOUT_TICKS + 1)


/******************************************************************************
 **                      STATIC VARIABLES
 *******************************************************************************/
static _Bool s_shouldHit;
static unsigned int s_triggerCounter = 0;

void Watchdog_init()
{
	s_shouldHit = true;
	WatchdogTimer1ModuleClkConfig();
	WatchdogTimerReset(SOC_WDT_1_REGS);
	WatchdogTimerDisable(SOC_WDT_1_REGS);
	WatchdogTimerPreScalerClkDisable(SOC_WDT_1_REGS);
	WatchdogTimerReloadSet(SOC_WDT_1_REGS, WD_RESET_VALUE);
	WatchdogTimerEnable(SOC_WDT_1_REGS);
}

void Watchdog_hit(void)
{
	// Hit the WD, giving it a new trigger each time to keep it from
	// resetting the board.
	s_triggerCounter++;

	WatchdogTimerTriggerSet(SOC_WDT_1_REGS, s_triggerCounter);
}

_Bool Watchdog_shouldHit(void)
{
	return s_shouldHit;
}

void Watchdog_stopHitting(void)
{
	s_shouldHit = false;
}