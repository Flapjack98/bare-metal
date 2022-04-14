/* The LED module is responsible for providing an abstract interface to
 * turn the BBG LEDS on and off */

#ifndef LED_GAURD_H
#define LED_GAURD_H

// Mapping from LED number to pin number
typedef enum Led_eLedNum {
    LED0_PIN = 21,
    LED1_PIN = 22,
    LED2_PIN = 23,
    LED3_PIN = 24,
} Led_eLedNum;

void Led_init(void);
void Led_unflashLed(Led_eLedNum ledPosition);
void Led_flashLed(Led_eLedNum ledPosition);

#endif