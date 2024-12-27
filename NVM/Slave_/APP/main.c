/*
 * main.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this main file is to implement a simple embedded application where button presses control the state of a system,
 *      and the system responds by changing its behavior (e.g., incrementing or decrementing a counter and controlling LEDs).
 */

/***********************************************
 * Includes
 ***********************************************/
#include <APP/scheduler.h>
#include "HAL/Initialization.h"



/***********************************************
 * Function Name: main
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: The main function initializes the system by calling the necessary
 *              initialization functions for the MCAL (Microcontroller Abstraction
 *              Layer), buttons, and LEDs. It enters an infinite loop where it
 *              continuously calls the `Scheduler()` function to manage state transitions
 *              and control the operation of LEDs based on the current state.
 ***********************************************/
int main(void) {

    INITIALIZATION_MCAL();
    INITIALIZATION_buttons();
    INITIALIZATION_leds();

    while(1) {

       Scheduler();

    }

}

