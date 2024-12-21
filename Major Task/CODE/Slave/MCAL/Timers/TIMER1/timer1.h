/*
 * timer1.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Declarations of Timer1-related functions and variables.
 *               2) A standardized interface for working with Timer1.
 *               3) Abstraction of hardware-specific details through function prototypes and preprocessor guards.
 */

#ifndef TIMER1_H_
#define TIMER1_H_


/***********************************************
 * Includes
 ***********************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_types.h"


/***********************************************
 * Shared Global Variables                     *
 ***********************************************/
volatile uint32_t g_ui32timer1Count;

/***********************************************
 * Functions Prototypes
 ***********************************************/
void TIMER1_init(void);
void TIMER1_start(void);
void TIMER1_reset(void);
void TIMER1_handler(void);

#endif /* TIMER1_H_ */
