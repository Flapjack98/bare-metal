/* The Joystick Handler module is responsible for handling up and down joystick
 * inputs from the zen cape to increase and decrease the speed of the leds
 * for the led flashing bare metal application. */

#include "joystick.h"
#include "ledFlasher.h"
#include "consoleUtils.h"
#include "hw_types.h"      // For _Bool

static _Bool s_joystickIsPressed[] = {
	false,
	false,
	false,
	false
};

static _Bool s_joystickWasReleased[] = {
	false,
	false,
	false,
	false
};

void JoystickHandler_doBackgroundWork(void)
{
    if (Joystick_readJoystick(Joystick_Up)) {
        s_joystickIsPressed[Joystick_Up] = true;
    } else if (s_joystickIsPressed[Joystick_Up]) {
        s_joystickIsPressed[Joystick_Up] = false;
        s_joystickWasReleased[Joystick_Up] = true;
    }

    if (Joystick_readJoystick(Joystick_Down)) {
        s_joystickIsPressed[Joystick_Down] = true;
    } else if (s_joystickIsPressed[Joystick_Down]) {
        s_joystickIsPressed[Joystick_Down] = false;
        s_joystickWasReleased[Joystick_Down] = true;
    }

    if (s_joystickWasReleased[Joystick_Up]) {
        s_joystickWasReleased[Joystick_Up] = false;
        LedFlasher_increaseSpeed();
    }
    
    if (s_joystickWasReleased[Joystick_Down]) {
        s_joystickWasReleased[Joystick_Down] = false;
        LedFlasher_decreaseSpeed();
    }
}