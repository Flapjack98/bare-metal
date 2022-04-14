/* The Joystick module provides an abstract interface to read the joystick
 * directions if they are pressed. NOTE: only Up and Down directions are
 * supported for reading at the moment to satisfy the application requirements.
 */

#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"      // For HWREG(...) macro
#include "watchdog.h"
#include "uart_irda_cir.h"
#include "consoleUtils.h"
#include "hw_cm_per.h"
#include "joystick.h"
#include <stdint.h>

/*****************************************************************************
**                INTERNAL STATIC TYPES AND VARIABLES
*****************************************************************************/

static struct pin_info_t {
	uint32_t base;
	uint8_t pin;
};

static struct pin_info_t JOYSTICK_PIN_INFOS[] = {
	{SOC_GPIO_2_REGS, 1}, // Left Joystick Button Pin
	{SOC_GPIO_1_REGS, 14}, // Down Joystick Button Pin
	{SOC_GPIO_1_REGS, 15}, // Right Joystick Button Pin
	{SOC_GPIO_0_REGS, 26} // Up Joystick Button Pin
};

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void initJoystickButtonPins(void);
static _Bool readButtonWithStarterWare(Joystick_eJoyStickDir joystickDir);

void Joystick_init(void)
{
	initJoystickButtonPins();
}

_Bool Joystick_readJoystick(Joystick_eJoyStickDir joystickDir)
{
	return readButtonWithStarterWare(joystickDir);
}

/*****************************************************************************
**                INTERNAL FUNCTION DEFINITIONS
*****************************************************************************/
void GPIO2ModuleClkConfig(void)
{

    /* Writing to MODULEMODE field of CM_PER_GPIO1_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
          CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
           CM_PER_GPIO2_CLKCTRL_MODULEMODE));
    /*
    ** Writing to OPTFCLKEN_GPIO_2_GDBCLK bit in CM_PER_GPIO2_CLKCTRL
    ** register.
    */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
          CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK;

    /*
    ** Waiting for OPTFCLKEN_GPIO_1_GDBCLK bit to reflect the desired
    ** value.
    */
    while(CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
           CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK));

    /*
    ** Waiting for IDLEST field in CM_PER_GPIO2_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_GPIO2_CLKCTRL_IDLEST_FUNC <<
           CM_PER_GPIO2_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
            CM_PER_GPIO2_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_GPIO_2_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK));
}

static void initJoystickButtonPins(void)
{
    /* Enabling functional clocks for GPIO instances. */
	GPIO0ModuleClkConfig();
	GPIO1ModuleClkConfig();
	GPIO2ModuleClkConfig();

    /* Selecting GPIO1[23] pin for use. */
    //GPIO1Pin23PinMuxSetup();

    /* Enabling the GPIO modules for the joystick pins. */
	GPIOModuleEnable(SOC_GPIO_0_REGS);
    GPIOModuleEnable(SOC_GPIO_1_REGS);
	GPIOModuleEnable(SOC_GPIO_2_REGS);

    /* Resetting the GPIO modules. */
	GPIOModuleReset(SOC_GPIO_0_REGS);
    GPIOModuleReset(SOC_GPIO_1_REGS);
	GPIOModuleReset(SOC_GPIO_2_REGS);

    /* Setting the joystick GPIO pins as an input pin. */
	GPIODirModeSet(JOYSTICK_PIN_INFOS[Joystick_Left].base,
                   JOYSTICK_PIN_INFOS[Joystick_Left].pin,
                   GPIO_DIR_INPUT);

    GPIODirModeSet(JOYSTICK_PIN_INFOS[Joystick_Down].base,
                   JOYSTICK_PIN_INFOS[Joystick_Down].pin,
                   GPIO_DIR_INPUT);
	
	GPIODirModeSet(JOYSTICK_PIN_INFOS[Joystick_Up].base,
                   JOYSTICK_PIN_INFOS[Joystick_Up].pin,
                   GPIO_DIR_INPUT);
	
	GPIODirModeSet(JOYSTICK_PIN_INFOS[Joystick_Left].base,
                   JOYSTICK_PIN_INFOS[Joystick_Left].pin,
                   GPIO_DIR_INPUT);
}

static _Bool readButtonWithStarterWare(Joystick_eJoyStickDir joystickDir)
{
	return GPIOPinRead(JOYSTICK_PIN_INFOS[joystickDir].base,
		JOYSTICK_PIN_INFOS[joystickDir].pin) == 0;
}