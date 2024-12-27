/*
 * timer1.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to configure and manage a timer (Timer1) on a microcontroller to generate an interrupt every 10 seconds.
 *      It provides functions to initialize, start, and reset Timer1.
 */


/***********************************************
 * Includes
 ***********************************************/
#include "timer1.h"



/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: Timer1_Init
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes Timer1 as a 32-bit periodic timer in one-shot mode. It configures the timer to generate an interrupt
 *              every second, based on the system clock. The function sets the load value for the timer to 1 second, registers
 *              the interrupt handler (`Timer1_Handler`), enables the timeout interrupt, and enables global interrupts. This
 *              setup is essential for creating time delays or periodic tasks in the system.
 ***********************************************/
void TIMER1_init(void) {
    // Enable the Timer1 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    // Wait for the peripheral to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)) {}

    // Configure Timer1 as a 32-bit timer in one-shot mode (for 10 seconds)
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    // Set the timer load value for 10 seconds (using system clock)
    uint32_t systemClock = SysCtlClockGet(); // Get the system clock frequency
    TimerLoadSet(TIMER1_BASE, TIMER_A, systemClock);  // 1 seconds interval

    // Register the Timer1 interrupt handler
    TimerIntRegister(TIMER1_BASE, TIMER_A, TIMER1_handler);

    // Enable the timeout interrupt for Timer1
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // Enable global interrupts
    IntMasterEnable();
}



/***********************************************
 * Function Name: Timer1_Start
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Starts the Timer1 by enabling it to begin counting. This function triggers the timer to start the counting
 *              process, which will eventually generate an interrupt after the specified time interval (10 seconds in this case).
 ***********************************************/
void TIMER1_start(void) {
    // Start the timer for 10-second count
    TimerEnable(TIMER1_BASE, TIMER_A);  // Enable the timer to start counting
}

/***********************************************
 * Function Name: Timer1_Reset
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Resets Timer1 to count for another 10-second interval. It reloads the timer with a new value based on the
 *              system clock frequency (SysCtlClockGet() * 10), which ensures a 10-second count. After resetting, it enables
 *              Timer1 to start the countdown again. This function is useful for restarting the timer and triggering the
 *              associated interrupt after the reset period.
 ***********************************************/
void TIMER1_reset(void) {
    // Reset Timer1 for another 10-second count
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() * 10);  // Reset for 10 seconds
    TimerEnable(TIMER1_BASE, TIMER_A);  // Enable Timer1 to start
}
