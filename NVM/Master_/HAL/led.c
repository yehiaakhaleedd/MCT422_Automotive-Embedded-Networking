/*
 * led.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to control the LEDs connected to specific GPIO pins.
 *      It defines functions to turn on and off the blue, green, red, and white LEDs connected to GPIO pins of Port F.
 */


/***********************************************
 * Includes
 ***********************************************/
#include "led.h"


/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: Blue_ON
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns on the blue LED connected to GPIO Port F, Pin 2. The function sets the GPIO pin high to light up the blue LED.
 ***********************************************/
void LED_blueOn(void) { GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); }


/***********************************************
 * Function Name: Blue_OFF
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns off the blue LED connected to GPIO Port F, Pin 2. The function sets the GPIO pin low to turn off the blue LED.
 ***********************************************/
void LED_blueOff(void) { GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); }


/***********************************************
 * Function Name: Green_ON
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns on the green LED connected to GPIO Port F, Pin 3. The function sets the GPIO pin high to light up the green LED.
 ***********************************************/
void LED_greenOn(void) { GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); }


/***********************************************
 * Function Name: Green_OFF
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns off the green LED connected to GPIO Port F, Pin 3. The function sets the GPIO pin low to turn off the green LED.
 ***********************************************/
void LED_greenOff(void) { GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); }


/***********************************************
 * Function Name: Red_ON
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns on the red LED connected to GPIO Port F, Pin 1. The function sets the GPIO pin high to light up the red LED.
 ***********************************************/
void LED_redOn(void) { GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); }


/***********************************************
 * Function Name: Red_OFF
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns off the red LED connected to GPIO Port F, Pin 1. The function sets the GPIO pin low to turn off the red LED.
 ***********************************************/
void LED_redOff(void) { GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); }


/***********************************************
 * Function Name: White_ON
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns on the white LED by setting the corresponding GPIO pins (Pins 1, 2, and 3) of GPIO Port F high.
 * This lights up the red, green, and blue LEDs simultaneously, resulting in a white LED light.
 ***********************************************/
void LED_whiteOn(void) {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
}


/***********************************************
 * Function Name: White_OFF
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Turns off the white LED by setting the corresponding GPIO pins (Pins 1, 2, and 3) of GPIO Port F low.
 * This turns off the red, green, and blue LEDs, resulting in no light output.
 ***********************************************/
void LED_whiteOff(void) {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
}



/***********************************************
 * Function Name: LED_initPortF
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes GPIO Port F for LED control. Enables the GPIOF
 *              peripheral and configures pins 1, 2, and 3 as output pins
 *              for driving the LEDs.
 ***********************************************/
void LED_initPortF(void) {
    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configure Port F pins 1, 2, and 3 as output (for LEDs)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}


/***********************************************
 * Function Name: LED_redBlink
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Controls the red LED to blink with a 1-second ON and 1-second
 *              OFF interval. Utilizes the TIMER0_delayMs function for delays.
 ***********************************************/
void LED_redBlink(void)
{
    LED_redOn();
    TIMER0_delayMs(1000);
    LED_redOff();
    TIMER0_delayMs(1000);
}

