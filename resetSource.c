#include "resetSource.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"
#include "wdtimer.h"

/*****************************************************************************
 **                Static Constants
 *****************************************************************************/
static const unsigned int REGISTERS_BASE_ADDRESS = (0x44E00F00);
static const unsigned int RESET_SOURCE_OFFSET = (0x8);
static const unsigned int RESET_SOURCE_REGISTER_ADDRESS = 
(REGISTERS_BASE_ADDRESS + RESET_SOURCE_OFFSET);

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

static void ResetSource_clearResetEventBit(int resetEventBit)
{
    
}

static char *ResetSource_readFromResetSource(void)
{
    char resetSourceListString[MAX_BUFFER_SIZE];
    for (int i = 0; i <= NUMBER_RESET_EVENT_TYPES; i++){
        // Get the pin
        int resetEventMask = (1<<ResetEventInfo[i].bit);
        // Apply the mask
        int result = HWREG(REGISTERS_BASE_ADDRESS + RESET_SOURCE_OFFSET) |= resetEventMask;
        // Check to see if bit is 1
        if (result) {
            // Make a string of of the resets found
            char resetEventType = ResetEventInfo[i].resetEventType;
            strcat(resetSourceListString, source);
            strcat(resetSourceListString, ", ");
        }
        ResetSource_clearResetEventBit(ResetEventInfo[i].bit);
    }

    return resetSourceListString;
}

void ResetSource_displayResetSource(void)
{
	char *resetSource = ResetSource_readFromResetSource();
}