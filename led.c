#include "led.h"
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"      // For HWREG(...) macro

/*****************************************************************************
 **                INTERNAL CONSTANTS
 *****************************************************************************/
static const unsigned int LED_GPIO_BASE = SOC_GPIO_1_REGS;

/*****************************************************************************
 **                PUBLIC FLASHING FUNCTIONS
 *****************************************************************************/

void Led_init(void)
{
    /* Enabling functional clocks for GPIO1 instance. */
	GPIO1ModuleClkConfig();

	/* Selecting GPIO1[23] pin for use. */
	//GPIO1Pin23PinMuxSetup();

	/* Enabling the GPIO module. */
	GPIOModuleEnable(LED_GPIO_BASE);

	/* Resetting the GPIO module. */
	GPIOModuleReset(LED_GPIO_BASE);

	/* Setting the GPIO pin as an output pin. */
	GPIODirModeSet(LED_GPIO_BASE,
			LED0_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED1_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED2_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED3_PIN,
			GPIO_DIR_OUTPUT);
}

void Led_unflashLed(Led_eLedNum ledPosition)
{
	/* Driving a logic Low on the GPIO pin. */
	GPIOPinWrite(LED_GPIO_BASE, ledPosition, GPIO_PIN_LOW);
}

void Led_flashLed(Led_eLedNum ledPosition)
{
	/* Driving a logic HIGH on the GPIO pin. */
	GPIOPinWrite(LED_GPIO_BASE, ledPosition, GPIO_PIN_HIGH);
}