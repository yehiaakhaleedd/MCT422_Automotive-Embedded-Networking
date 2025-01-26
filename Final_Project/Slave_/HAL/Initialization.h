/*
 * initialization.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Serve as a central initialization interface for key system components.
 *               2) Abstract and organize initialization procedures for MCAL and buttons, simplifying system setup.
 *               3) Promote modularity and code reuse by encapsulating initialization routines in dedicated functions.
 */

#ifndef _INITIALIZATION_H_
#define _INITIALIZATION_H_


/***********************************************
 * Includes
 ***********************************************/
#include <MCAL/UART/uart.h>
#include "MCAL/SPI/spi.h"
#include "MCAL/Timers/TIMER0/timer0.h"
#include "MCAL/Timers/TIMER1/timer1.h"
#include "MCAL/Timers/SYSTICK_TIMER/systickTimer.h"
#include "MCAL/CAN/can.h"
#include "buttons.h"
#include "led.h"



/***********************************************
 * Functions Prototypes
 ***********************************************/
/*void INITIALIZATION_MCAL(void);
void INITIALIZATION_buttons(void);
void INITIALIZATION_leds(void);


void INITIALIZATION_startTimer1(void);
void INITIALIZATION_resetTimer1(void);

void TIMER0_delayMSec(uint32_t a_uint32delayMs);

void TIMER1_clearInt(void);






















void SPI_put(uint32_t a_uint32data);
bool SPI_check(void);*/

#endif /* _INITIALIZATION_H_ */
