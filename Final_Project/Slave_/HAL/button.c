/*
 * button.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to configure and manage the buttons and LEDs connected to GPIO Port F.
 *      It provides functions for initializing the GPIO pins associated with buttons for input and LEDs for output,
 *      as well as functions for checking the state of the buttons.
 */


/***********************************************
 * Includes
 ***********************************************/
#include "buttons.h"


/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: BUTTONS_rightButton
 * Inputs: N/A
 * Outputs: bool - Returns true if the right button is pressed (low state), otherwise false.
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Reads the state of the right button (connected to GPIO_PIN_0 on Port F).
 * Returns true if the button is pressed (low state), and false if it is not pressed (high state).
 ***********************************************/
bool BUTTONS_rightButton(void) { return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0; }



/***********************************************
 * Function Name: BUTTONS_leftButton
 * Inputs: N/A
 * Outputs: bool - Returns true if the left button is pressed (low state), otherwise false.
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Reads the state of the left button (connected to GPIO_PIN_4 on Port F).
 * Returns true if the button is pressed (low state), and false if it is not pressed (high state).
 ***********************************************/
bool BUTTONS_leftButton(void) { return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0; }

bool HAL_voidButtons(uint32_t Button)
{
    uint32_t state;

    switch(Button)
    {
    case RIGHT:state = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0;
        break;

    case LEFT:state = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0;
        break;

     default:
        break;

    }

    return state;
}


/***********************************************
 * Function Name: PortF_Init
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes Port F for the use of buttons (PF0, PF4) and LEDs (PF1, PF2, PF3).
 * - Enables GPIO Port F and configures the system clock to 50 MHz.
 * - Configures PF1, PF2, PF3 as output pins for LEDs.
 * - Unlocks and configures PF0 as an input for the right button with a pull-up resistor.
 * - Configures PF4 as an input for the left button with a pull-up resistor.
 ***********************************************/
void PortF_Init(void) {
    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


    // Configure Port F pins for LED and buttons
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


    // Step 3: Unlock Port F pin 0 (only required for PF0, as it is locked by default)
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   // Unlock Port F
        HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;       // Commit register for PF0
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;               // Re-lock Port F

        // Step 4: Configure pins 0 and 4 as input with pull-up resistors
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
        GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

/***********************************************
 * Function Name: BUTTONS_initPortF
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes GPIO Port F for button control. Unlocks and configures
 *              pin 0 (PF0) and configures both pins 0 and 4 (PF0 and PF4) as input
 *              with pull-up resistors for button functionality.
 ***********************************************/
void BUTTONS_initPortF(void) {
    // Unlock Port F pin 0 (only required for PF0, as it is locked by default)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   // Unlock Port F
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;       // Commit register for PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;               // Re-lock Port F

    // Configure pins 0 and 4 as input with pull-up resistors (for buttons)
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


