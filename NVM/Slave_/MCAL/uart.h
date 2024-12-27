/*
 * uart.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) It declares the functions related to UART initialization, configuration, data transmission, and reception.
 *               2) It contains macros for configuration settings, such as baud rates, register addresses, and control bits.
 *               3) It abstracts the low-level hardware details, enabling developers to use simple APIs without dealing directly with registers.
 */

#ifndef UART_H_
#define UART_H_


/***********************************************
 * Includes
 ***********************************************/
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"


/***********************************************
 * Definitions and Macros
 ***********************************************/
#define STATE_RED     0x01
#define STATE_GREEN   0x02
#define STATE_BLUE    0x03
#define STATE_WHITE   0x04
#define COMM_LOST     0x05
#define UNIDENTIFIED  0x30
#define DONT_TOUCH    0x40

/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/
uint32_t data;
bool initialMessageSent;


/***********************************************
 * Functions Prototypes
 ***********************************************/
void UART0_init(void);
void UART0_sendMessage(const char *array_ptr);

#endif /* UART_H_ */
