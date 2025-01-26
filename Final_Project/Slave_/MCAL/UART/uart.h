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
#include "MCAL/UART/uart_config.h"


/***********************************************
 * Definitions and Macros
 ***********************************************/
#define STATE_RED     0x01U
#define STATE_GREEN   0x02U
#define STATE_BLUE    0x03U
#define STATE_WHITE   0x04U
#define COMM_LOST     0x05U
#define STATE_INIT    0x06U
#define RECEIVED_COMM 0x22U
//#define UNIDENTIFIED  0x30
#define LEAVE_BUTTON  0x33U
//#define DONT_TOUCH    0x40


#define UART_MODULE_0      0x00
#define UART_MODULE_1      0x01


/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/
uint32_t data;
bool initialMessageSent;
uint32_t UartState;
extern uint32_t UartCurrentState;

/***********************************************
 * Functions Prototypes
 ***********************************************/
void UART_Init(void);
void UART_SendMessage(const char *message);
void UART0_sendMessage(const char *array_ptr);
void UART0_init(void);
void UART_SendNumber(uint8_t number);

#endif /* UART_H_ */
