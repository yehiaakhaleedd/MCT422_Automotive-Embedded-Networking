/*
 * scheduler.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Define and manage system states for controlling the flow of operations.
 *               2) Provide an abstraction for communication checking and task scheduling.
 *               3) Serve as a centralized location for handling state-based functionality in the application.
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_


/***********************************************
 * Includes
 ***********************************************/
#include <HAL/initialization.h>
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
 * Definitions and Macros
 ***********************************************/
#define EEPROM_COUNTER_ADDRESS 0x00


/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/
bool unidentifiedflag;
bool recievedFlag;
bool holdButtonFlag;
bool sw1; // Right button
bool sw2; // Left button
extern bool APP_uint32last_sw1_state; // Declare as external
extern bool APP_uint32last_sw2_state;
extern uint32_t APP_uint32counter;

/***********************************************
 * Functions Prototypes
 ***********************************************/
void APP_sendToPc(uint32_t a_uint32data);
void IncrementCounter(void);

#endif /* SCHEDULER_H_ */
