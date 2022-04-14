/* The Reset Source module reads and displays the reset source register and 
 * displays a reading of the reasons/sources for reset since last read. Once the
 * sources are read, the module clears the register and sets it back to a 
 * default state before any resets occured. */

#ifndef RESETSOURCE_GAURD_H
#define RESETSOURCE_GAURD_H


// Displays the sources of the last reset(s) since the last read of the reset
// source event register.
void ResetSource_displayResetSource(void);
#endif