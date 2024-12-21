/*
 * spi.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Provide function prototypes for SPI initialization and communication.
 *               2) Abstract hardware-specific SPI details for easier implementation.
 *               3) Facilitate master-slave communication and acknowledge signaling.
 *               4) Define shared global variables and flags for SPI-based communication tasks.
 */

#ifndef SPI_H_
#define SPI_H_


/***********************************************
 * Includes
 ***********************************************/
#include "MCAL/UART/uart.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "inc/hw_gpio.h"


/***********************************************
 * Definitions and Macros
 ***********************************************/
#define ACK         0x27


/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/
uint32_t receivedData;
uint32_t receivedAck;
bool commcheck;
bool unidentifiedFlag;
bool SlaveButtonsFlag;
bool whiteFlag;


/***********************************************
 * Functions Prototypes
 ***********************************************/
void SPI_masterInit(void);
void SPI_slaveInit(void);
void SPI_masterSend(uint32_t a_uint32counter);



#endif /* SPI_H_ */
