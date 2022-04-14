#include "resetSource.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"
#include "wdtimer.h"

/*****************************************************************************
 **                Internal Macro Definitions
 *****************************************************************************/
#define RESET_SOURCE_REG_ADDR (0x4030CE4C)
#define RESET_SOURCE_OFFSET (0x8)

/*****************************************************************************
 **                Static Constants
 *****************************************************************************/
static const int MAX_BUFFER_SIZE = 1024;
static const int NUMBER_RESET_EVENT_TYPES = 5;

// Reset events
typedef struct ResetEvent {
    int bit;
    char *resetEventType;
} ResetEvent;

static ResetEvent ResetEventInfo[] = {
    {9, "Ice Pick"},
    {5, "External Warm"},
    {4, "Watchdog Timer"},
    {1, "GLobal Warm Software"},
    {0, "Powen-on (cold)"}
}

static char *ResetSource_readFromResetSource(void)
{
    char resetSourceListString[MAX_BUFFER_SIZE];
    for (int i = 0; i <= NUMBER_RESET_EVENT_TYPES; i++){
        // Get the pin
        int resetEventMask = (1<<ResetEventInfo[i].bit);
        // Apply the mask
        int result = HWREG(RESET_SOURCE_REG_ADDR + RESET_SOURCE_OFFSET) |= resetEventMask;
        // Check to see if bit is 1
        if (result) {
            // Make a string of of the resets found
            char resetEventType = ResetEventInfo[i].resetEventType;
            strcat(resetSourceListString, source);
            strcat(resetSourceListString, ", ");
        }
    }

    return resetSourceListString;
}

void ResetSource_displayResetSource(void)
{
	char *resetSource = ResetSource_readFromResetSource();
}