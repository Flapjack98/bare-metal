/* This Timer module is responsible for creating a bare metal timer using
 * interrupts for its implementation.
 * The interval may be specified in milliseconds.
 * NOTE: This module assumes that the timer being used is
 * 32-bits in size and has a 24MHz clock.
 * Code is adapted from the FakeTyper example on the course website:
 * https://opencoursehub.cs.sfu.ca/bfraser/solutions/433/15-BareMetal/bm_faketyper/*/

#include "soc_AM335x.h"
#include "beaglebone.h"
#include "error.h"
#include "interrupt.h"
#include "dmtimer.h"
#include "timer.h"
#include <stdint.h>

static void Timer_dmTimerAintcConfigure(void);
static void Timer_dmTimerSetUp(uint32_t periodMilliseconds);
static void Timer_dmTimerIsr(void);

static uint32_t Timer_calcInitialCountFromPeriodMilliseconds(
    uint32_t periodMilliseconds);

static volatile _Bool g_flagIsr = 0;
static void (*g_isrCallback)(void) = 0;

void Timer_init(unsigned int timerIntervalMilliseconds)
{
	/* This function will enable clocks for the DMTimer2 instance */
	DMTimer2ModuleClkConfig();

	/* Enable IRQ in CPSR */
	IntMasterIRQEnable();

	/* Register DMTimer2 interrupts on to AINTC */
	Timer_dmTimerAintcConfigure();

	/* Perform the necessary configurations for DMTimer */
	Timer_dmTimerSetUp(timerIntervalMilliseconds);

	// Extra setup:
	// ..Select clock
	// ..Set prescaler
	DMTimerPreScalerClkDisable(SOC_DMTIMER_2_REGS);

	/* Enable the DMTimer interrupts */
	DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);

	/* Start the DMTimer */
	DMTimerEnable(SOC_DMTIMER_2_REGS);
}

void Timer_cleanup(void)
{
    /* Stop the DMTimer */
	DMTimerDisable(SOC_DMTIMER_2_REGS);
}

_Bool Timer_isIsrFlagSet(void)
{
	return g_flagIsr;
}
void Timer_clearIsrFlag(void)
{
	g_flagIsr = false;
}

void Timer_setTimerIsrCallback(void (*isrCallback)(void))
{
	g_isrCallback = isrCallback;
}

// Timer ISR
static void Timer_dmTimerIsr(void)
{
	/* Disable the DMTimer interrupts */
	DMTimerIntDisable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);

	/* Clear the status of the interrupt flags */
	DMTimerIntStatusClear(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_IT_FLAG);

	// --------------------------------------------------------
	// Work done in the ISR:
	// .. Record ISR state for background task to poll.
	g_flagIsr = true;

	// .. Call user's ISR function (if any)
	if (g_isrCallback != 0) {
		g_isrCallback();
	}

	// --------------------------------------------------------

	/* Enable the DMTimer interrupts */
	DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
}

static void Timer_dmTimerAintcConfigure(void)
{
	/* Initialize the ARM interrupt control */
	// NOTE: Must disable this, otherwise clears any already established
	// interrupts, such as the serial Rx.
	//IntAINTCInit();

	/* Registering Timer_dmTimerIsr */
	IntRegister(SYS_INT_TINT2, Timer_dmTimerIsr);

	/* Set the priority */
	IntPrioritySet(SYS_INT_TINT2, 0, AINTC_HOSTINT_ROUTE_IRQ);

	/* Enable the system interrupt */
	IntSystemEnable(SYS_INT_TINT2);
}

static void Timer_dmTimerSetUp(uint32_t periodMilliseconds)
{
    /* Get the initial count for the timer */
    uint32_t initialCount = Timer_calcInitialCountFromPeriodMilliseconds(
        periodMilliseconds);

	/* Load the counter with the initial count value */
	DMTimerCounterSet(SOC_DMTIMER_2_REGS, initialCount);

	/* Load the load register with the reload count value */
	DMTimerReloadSet(SOC_DMTIMER_2_REGS, initialCount);

	/* Configure the DMTimer for Auto-reload and compare mode */
	DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
}

// Calculates the TLDR (initial count) for a 32-bit timer with a 24 MHz clock,
// given a period that is in milliseconds (periodMilliseconds).
static uint32_t Timer_calcInitialCountFromPeriodMilliseconds(
    uint32_t periodMilliseconds)
{
    const int CLOCK_MEGAHERTZ = 24;
    const int METRIC_CONVERSION_FACTOR = 1000;
    const int OVERFLOW_OFFSET = 1;
    const uint32_t MAX_32_BIT_VALUE = 0xFFFFFFFF;

    // Formula: Max32BitsVal -
    // (periodMilliseconds*clockMegahertz*MetricConversionFactor) + Overflow1
    return (MAX_32_BIT_VALUE - (periodMilliseconds
        * CLOCK_MEGAHERTZ * METRIC_CONVERSION_FACTOR)) + OVERFLOW_OFFSET;
}