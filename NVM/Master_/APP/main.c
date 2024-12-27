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
#include <HAL/initialization.h>



/***********************************************
 * Function Name: main
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: The main function initializes the system by calling the necessary
 *              initialization functions for the MCAL (Microcontroller Abstraction
 *              Layer), buttons, and LEDs. It enters an infinite loop to keep the
 *              program running, allowing the system to continuously perform tasks
 *              based on the initialization and interrupt handlers.
 ***********************************************/
int main(void)
{
    //SPI_masterSend(0x09);
    INITIALIZATION_MCAL();
    INITIALIZATION_buttons();
    INITIALIZATION_leds();

    while(1) {

    }

}




