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
#include <OS/scheduler.h>
//#include <HAL/initialization.h>



/***********************************************
 * Function Name: main
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Main function of the program.
 *              Enters an infinite loop to call the operating system (OS) function.
 ***********************************************/
int main(void)
{
    while(1) {
        APP_voidOS();
    }
}




