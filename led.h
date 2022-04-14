// led.h:
// 

#ifndef _LED_H_
#define _LED_H_

void Led_init(void);
void Led_setSpeed(const char *newDirection);
void Led_notifyOnTimeIsr(void);
void Led_doBackgroundWork(void);


#endif
