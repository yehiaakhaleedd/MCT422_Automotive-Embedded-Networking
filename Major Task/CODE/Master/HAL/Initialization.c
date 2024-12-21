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
#include <HAL/initialization.h>

/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: init_MCAL
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
    SPI_masterInit();
    UART_Init();
    TIMER0_init();
    SYSTICK_init();
    CAN_Init();
    UART0_init();
}



**********************************************
 * Function Name: initButtons
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
 * Function Name: SPI_send
 * Inputs: uint32_t a_uint32counter - The data to be sent via SPI
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Sends a 32-bit value through SPI by calling the
 *              SPI_masterSend function.
 **********************************************
void SPI_send(uint32_t a_uint32counter)
{
    SPI_masterSend(a_uint32counter);
}

**********************************************
 * Function Name: UART0_send
 * Inputs: const char *array_ptr - Pointer to the character array (string) to be sent
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Sends a string through UART0 by calling the UART0_sendMessage function.
 **********************************************
void UART0_send(const char *array_ptr)
{
    UART_SendMessage(array_ptr);
    //UART0_sendMessage(array_ptr);
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


