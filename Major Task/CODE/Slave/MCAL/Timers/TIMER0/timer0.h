/*
 * timer0.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Provide function prototypes for initializing Timer0 and creating delays in milliseconds.
 *               2) Abstract hardware-specific details, offering a clean, modular interface.
 *               3) Enable reuse of Timer0 functionality in other parts of the program without needing to deal with low-level register manipulations.
 */

#ifndef TIMER0_H_
#define TIMER0_H_


/***********************************************
 * Includes
 ***********************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"


/***********************************************
 * Functions Prototypes
 ***********************************************/
void TIMER0_init(void);
void TIMER0_delayMs(uint32_t a_uint32delayMs);


#endif
