// joystick.h:
// 

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

void Joystick_init(void);
void Joystick_setDirection(const char *newDirection);
void Joystick_notifyOnTimeIsr(void);
void Joystick_doBackgroundWork(void);


#endif
