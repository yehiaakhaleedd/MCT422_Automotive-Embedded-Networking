/*
 * systickTimer.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Provide function prototypes and global variables for configuring and handling SysTick timer functionality.
 *               2) Abstract low-level hardware details related to the SysTick timer.
 *               3) Enable developers to use the SysTick timer for system-level timing tasks, such as time delays or periodic interrupts.
 */

#ifndef SYSTICKTIMER_H_
#define SYSTICKTIMER_H_


/***********************************************
 * Includes
 ***********************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"


/***********************************************
 * Shared Global Variables                     *
 ***********************************************/
volatile uint32_t g_ui32SysTickCount ;


/***********************************************
 * Functions Prototypes                        *
 ***********************************************/
void SYSTICK_init(void);
void SYSTICK_handler(void);


#endif /* SYSTICKTIMER_H_ */
