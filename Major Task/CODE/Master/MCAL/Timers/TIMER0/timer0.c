/*
 * timer0.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to configure and use Timer0 to provide a delay function based on a specified time in milliseconds.
 *      It enables the use of Timer0 for timing purposes, specifically to create a delay in the program execution.
 */


/***********************************************
 * Includes
 ***********************************************/
#include <MCAL/Timers/TIMER0/timer0.h>



/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: Timer0_Init
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes Timer0 as a 32-bit one-shot timer. It enables the Timer0 peripheral, waits for it to be ready,
 *              and configures it in one-shot mode where the timer counts up once until it reaches the load value. This setup
 *              allows the timer to be used for a single event or one-time delay.
 ***********************************************/
void TIMER0_init(void) {
    // Enable the Timer0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Wait for the peripheral to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) {}

    // Configure Timer0 as a 32-bit timer in periodic mode
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT_UP);
}



/***********************************************
 * Function Name: Timer0_DelayMs
 * Inputs: uint32_t a_uint32delayMs - The delay in milliseconds for which the timer should count.
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function creates a delay by using Timer0. It calculates the required number of timer ticks based on
 *              the system clock frequency and the desired delay in milliseconds. It loads the timer with the calculated
 *              ticks, clears any previous timeout flag, and starts the timer. The function then waits for the timer to
 *              timeout (i.e., the specified delay duration), and once the delay is completed, it disables Timer0.
 *              This is typically used for generating precise time delays in a program.
 ***********************************************/
void TIMER0_delayMs(uint32_t a_uint32delayMs) {
    uint32_t clockFrequency = SysCtlClockGet();
    uint32_t timerTicks = (clockFrequency / 1000) * a_uint32delayMs;

    // Load the timer with the required number of ticks
    TimerLoadSet(TIMER0_BASE, TIMER_A, timerTicks - 1);

    // Clear the Timer0 timeout flag
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable Timer0
    TimerEnable(TIMER0_BASE, TIMER_A);

    // Wait until the timer counts down
    while ((TimerIntStatus(TIMER0_BASE, false) & TIMER_TIMA_TIMEOUT) == 0) {}

    // Disable Timer0
    TimerDisable(TIMER0_BASE, TIMER_A);
}
