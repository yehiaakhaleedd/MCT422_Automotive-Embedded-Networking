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
#define MAX_ELEMENTS     30

#define EEPROM_COUNTER_ADDRESS 0x00

/***********************************************
 * Shared Global Variables                     *
 ***********************************************/


/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/

extern bool APP_boolWhiteFlag;
extern bool APP_boolWhiteStateFlag;

extern uint32_t APP_uint32CurrentStateIndex;
extern uint32_t APP_uint32checkcounter;
extern uint32_t APP_uint32i;

uint32_t receivedData;
uint32_t counter;
uint32_t CurrentState;
uint32_t StoredValues[MAX_ELEMENTS];

/***********************************************
 * Functions Prototypes
 ***********************************************/
void Scheduler(void);
void IncrementCounter(void);

#endif /* SCHEDULER_H_ */
