#include "timer1.h"

/***********************************************
 * Function Name: TIMER1_init
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes Timer1 as a 32-bit periodic timer and configures
 *              it for a 10-second interval. Sets up the necessary interrupts
 *              and enables the Timer1 peripheral.
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
 * Function Name: TIMER1_start
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Starts Timer1 to begin counting for a 10-second interval.
 *              Enables the timer for counting without modifying any flags.
 ***********************************************/
void TIMER1_start(void) {
    // Start the timer for 10-second count
    TimerEnable(TIMER1_BASE, TIMER_A);  // Enable the timer to start counting
}

/***********************************************
 * Function Name: TIMER1_reset
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Resets Timer1 to its initial state and starts a new 10-second
 *              counting interval.
 ***********************************************/
void TIMER1_reset(void) {
    // Reset Timer1 for another 10-second count
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() * 10);  // Reset for 10 seconds
    TimerEnable(TIMER1_BASE, TIMER_A);  // Enable Timer1 to start
}
