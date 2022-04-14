#include "consoleUtils.h"
#include "resetSource.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"
#include "wdtimer.h"
#include <stdlib.h>
#include <stdio.h>

/*****************************************************************************
 **                Static Constants
 *****************************************************************************/
static const unsigned int REGISTERS_BASE_ADDRESS = (0x44E00F00);
static const unsigned int RESET_SOURCE_OFFSET = (0x8);
static const unsigned int RESET_SOURCE_REGISTER_ADDRESS = (0x44E00F08); // (REGISTERS_BASE_ADDRESS + RESET_SOURCE_OFFSET)

static const int MAX_BUFFER_SIZE = 1024;
static const int NUMBER_RESET_EVENT_TYPES = 5;

/*****************************************************************************
 **                Reset Source Event Stuct and Info List
 *****************************************************************************/
typedef struct ResetEvent {
    int bit;
    char *resetEventType;
} ResetEvent;

static ResetEvent ResetEventInfo[] = {
    {9, "Ice Pick"},
    {5, "External Warm"},
    {4, "Watchdog Timer"},
    {1, "GLobal Warm Software"},
    {0, "Power-on (cold)"}
};

/*****************************************************************************
 **                Private Functions
 *****************************************************************************/

static void ResetSource_clearResetEventBit(int resetEventBit)
{
    int resetEventMask = (1<<resetEventBit);
    HWREG(RESET_SOURCE_REGISTER_ADDRESS) |= resetEventMask;
}

static char *ResetSource_readFromResetSource(void)
{
    int i;
    char *resetSourceListString = malloc(sizeof(char)*MAX_BUFFER_SIZE);
    for (i = 0; i <= NUMBER_RESET_EVENT_TYPES; i++){
        // Get the pin
        int resetEventMask = (1<<ResetEventInfo[i].bit);
        // Apply the mask
        // Check to see if bit is 1
        int result = HWREG(RESET_SOURCE_REGISTER_ADDRESS) & (resetEventMask);
        if (result) { // if result > 0 that means that a reset has occured
            // Make a string of of the resets found
            char *lastResetSourceListString = resetSourceListString;
            char *resetEventType = ResetEventInfo[i].resetEventType;
            snprintf(resetSourceListString, MAX_BUFFER_SIZE, "%s, %s", lastResetSourceListString, resetEventType);
        }
        ResetSource_clearResetEventBit(ResetEventInfo[i].bit);
    }

    return resetSourceListString;
}

/*****************************************************************************
 **                Public Functions
 *****************************************************************************/

void ResetSource_displayResetSource(void)
{
	char *resetSource = ResetSource_readFromResetSource();
    ConsoleUtilsPrintf("\n%s\n", resetSource);
    free(resetSource);
}
