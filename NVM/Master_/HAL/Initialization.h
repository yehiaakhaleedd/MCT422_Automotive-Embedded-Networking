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
#include <MCAL/uart.h>
#include "MCAL/spi.h"
#include "MCAL/timer0.h"
#include "MCAL/timer1.h"
#include "MCAL/systickTimer.h"
#include "led.h"
#include "buttons.h"
#include "driverlib/eeprom.h"


/***********************************************
 * Functions Prototypes
 ***********************************************/
void INITIALIZATION_MCAL(void);
void INITIALIZATION_buttons(void);
void INITIALIZATION_leds(void);


void TIMER1_clearInt(void);

void SPI_send(uint32_t a_uint32counter);

void UART0_send(const char *array_ptr);


#endif /* _INITIALIZATION_H_ */
