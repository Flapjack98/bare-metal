/* The Joystick module provides an abstract interface to read the joystick
 * directions if they are pressed. NOTE: only Up and Down directions are
 * supported for reading at the moment to satisfy the application requirements.
 */

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

typedef enum Joystick_eJoyStickDir {
	Joystick_Left,
	Joystick_Down,
	Joystick_Right,
	Joystick_Up
} Joystick_eJoyStickDir;

void Joystick_init(void);
_Bool Joystick_readJoystick(Joystick_eJoyStickDir joystickDir);


#endif
