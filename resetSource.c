#include "resetSource.h"

static char *ResetSource_readFromResetSource(void)
{
    return "test";
}

void ResetSource_displayResetSource(void)
{
	char *resetSource = ResetSource_readFromResetSource();
}