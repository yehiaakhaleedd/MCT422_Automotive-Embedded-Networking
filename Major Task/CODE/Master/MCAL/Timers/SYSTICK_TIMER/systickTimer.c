/*
 * systickTimer.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to configure the SysTick timer on a microcontroller to generate periodic interrupts.
 *      Specifically, it sets up the SysTick timer to trigger an interrupt at regular intervals (every 50 milliseconds).
 */


/***********************************************
 * Includes
 ***********************************************/
#include <MCAL/Timers/SYSTICK_TIMER/systickTimer.h>

/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: SysTick_Init
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the SysTick timer with a 50ms interval based on the system clock frequency. It sets the SysTick
 *              period, registers the SysTick interrupt handler (`SysTick_Handler`), enables the SysTick interrupt, and
 *              finally enables the SysTick timer itself. This function is typically used to set up periodic interrupts for
 *              time-based operations.
 ***********************************************/
void SYSTICK_init(void) {
    // Set the clock source for SysTick to the system clock (default)
    SysTickPeriodSet(SysCtlClockGet() / 1000);  // 1ms interval
    SysTickIntRegister(SYSTICK_handler);        // Register SysTick interrupt handler
    SysTickIntEnable();                         // Enable SysTick interrupt
    SysTickEnable();                            // Enable SysTick timer
}
