/*
 * initialization.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to initialize various hardware modules and peripherals in the microcontroller system, setting up communication interfaces and essential system timers.
 *      Specifically, it provides functions to initialize MCAL which includes peripheral initialization for communication and timing.
 *      It also initializes a GPIO port which is likely for button input for the user interface.
 */


/***********************************************
 * Includes
 ***********************************************/
#include "initialization.h"

/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: INITIALIZATION_MCAL
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the MCAL by calling the necessary functions for SPI, UART, Timer0, and SysTick initialization.
 * This function ensures that all peripherals and timers are properly configured and ready for use in the application.
 ***********************************************/
/*
void INITIALIZATION_MCAL(void)
{
    SPI_slaveInit();
    UART0_init();
    TIMER0_init();
    TIMER1_init();
    SYSTICK_init();
    CAN_Init();
    CAN_ReceiveInit();
}

**********************************************
 * Function Name: INITIALIZATION_buttons
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the buttons by calling the PortF_Init function.
 * This function configures the GPIO pins for button inputs on Port F.
 **********************************************
void INITIALIZATION_buttons(void)
{
    BUTTONS_initPortF();
}

**********************************************
 * Function Name: INITIALIZATION_startTimer1
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Calls the Timer1_Start function to initiate Timer1 and start
 *              counting for a 10-second interval.
 **********************************************
void INITIALIZATION_startTimer1(void)
{
    TIMER1_start();
}

**********************************************
 * Function Name: INITIALIZATION_resetTimer1
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Calls the Timer1_Reset function to reset Timer1 and start a
 *              new 10-second counting interval.
 **********************************************
void INITIALIZATION_resetTimer1(void)
{
    TIMER1_reset();
}

**********************************************
 * Function Name: TIMER0_delayMSec
 * Inputs: uint32_t a_uint32delayMs - Delay duration in milliseconds
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Creates a delay for the specified duration in milliseconds
 *              by calling the TIMER0_delayMs function.
 **********************************************
void TIMER0_delayMSec(uint32_t a_uint32delayMs)
{
    TIMER0_delayMs(a_uint32delayMs);
}

**********************************************
 * Function Name: TIMER1_clearInt
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Clears the interrupt flag for Timer1 to acknowledge the
 *              timeout interrupt and prevent it from triggering again.
 **********************************************
void TIMER1_clearInt(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}


**********************************************
 * Function Name: SPI_put
 * Inputs: uint32_t a_uint32data - The 32-bit data to be sent via SPI
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Sends a 32-bit data value through SPI by calling the SPI_putData function.
 **********************************************
void SPI_put(uint32_t a_uint32data)
{
    SPI_putData(a_uint32data);
}


**********************************************
 * Function Name: SPI_check
 * Inputs: N/A
 * Outputs: bool - The result of SPI_checkData indicating the status of SPI communication
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Checks the status of SPI communication by calling the SPI_checkData function
 *              and returns its result.
 **********************************************
bool SPI_check(void)
{
    return SPI_checkData();  // Return the result of SPI_checkData
}



**********************************************
 * Function Name: INITIALIZATION_leds
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the LEDs by calling the LED_initPortF function
 *              to configure the necessary GPIO pins for LED control.
 **********************************************
void INITIALIZATION_leds(void)
{
    LED_initPortF();
}
*/


