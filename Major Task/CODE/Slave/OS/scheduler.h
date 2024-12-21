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
//#include <HAL/initialization.h>
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
#include <MCAL/Timers/SYSTICK_TIMER/systickTimer.h>
#include <MCAL/Timers/TIMER0/timer0.h>
#include <MCAL/Timers/TIMER1/timer1.h>
#include <MCAL/UART/uart.h>
#include "MCAL/SPI/spi.h"
#include "MCAL/CAN/can.h"
#include "HAL/led.h"
#include "HAL/buttons.h"
#include "OS/OS_config.h"


/***********************************************
 * Definitions and Macros
 ***********************************************/
#define MAX_ELEMENTS     0x30U
#define CAN_TX_UNIDENTIFIED_MESSAGE_ID          0x09

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
uint8_t CurrentState;
uint32_t StoredValues[MAX_ELEMENTS];

/***********************************************
 * Functions Prototypes
 ***********************************************/
void Scheduler(void);
void APP_voidOS(void);
void OS_voidHeartBeat(void);
void OS_voidStateControl(uint32_t a_ui32counter);
void OS_voidCheckIdle(void);
void OS_voidCheckCANCommunication(void);
void OS_voidQueueControl(void);
void OS_voidCheckUnidentified(void);
void OS_voidCheckSlaveButtons(void);
void OS_voidCheckErrors(void);
void OS_voidAddTask(void (*taskFunction)(void), uint32_t periodTicks, uint8_t priority);
void OS_voidSortTasksByPriority(void);
void OS_voidSchedulerr(void);
void OS_voidInitTasks(void);
void APP_ui32CheckButtons(void);
void OS_voidTurnOnLED(void);
void OS_voidStateControll(void);
void OS_voidInit(void);
void OS_voidMCALInit(void);
void OS_voidHALInit(void);
void OS_voidInitAll(void);

void INITIALIZATION_MCAL(void);
void INITIALIZATION_buttons(void);
void INITIALIZATION_leds(void);
void SPI_put(uint32_t a_uint32data);
bool SPI_check(void);


#endif /* SCHEDULER_H_ */
