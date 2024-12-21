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


/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/
bool recievedFlag;
bool holdButtonFlag;
bool sw1; // Right button
bool sw2; // Left button

extern uint32_t APP_ui32UartCurrentState;
extern uint8_t APP_ui32counter;
extern uint32_t APP_ui32idleCounter;
extern uint32_t APP_ui32holdButton1count;
extern uint32_t APP_ui32holdButton2count;
extern uint32_t OS_ui32CurrentError;
extern uint32_t OS_ui32PrevError;
extern uint32_t OS_ui32index;

extern bool APP_boollast_sw1_state;
extern bool APP_boollast_sw2_state;
extern bool OS_boolidleFlag;
extern bool APP_boolStateInit;
extern bool APP_boolUnidentifiedFlag;

/***********************************************
 * Functions Prototypes
 ***********************************************/
void APP_voidSendToPc(uint32_t a_uint32data);
void APP_voidStateControll(uint32_t a_ui32counter);
void APP_voidUartControl(void);
void APP_voidCheckCommunication(void);
void APP_voidOS(void);
void APP_voidScheduler(void);
void APP_ui32CheckButtons(void);

void OS_voidCheckCANCommunication(void);
void OS_voidCheckIdle(void);
void OS_voidInit(void);
void OS_voidHALInit(void);
void OS_voidMCALInit(void);
void OS_voidCanSend(void);
void OS_voidAddTask(void (*taskFunction)(void), uint32_t periodTicks, uint8_t priority);
void OS_voidSortTasksByPriority(void);
void OS_voidSchedulerr(void);
void OS_voidInitTasks(void);
void APP_voidStateControl(void);

void INITIALIZATION_MCAL(void);
void INITIALIZATION_buttons(void);
void INITIALIZATION_leds(void);
void TIMER1_clearInt(void);
void SPI_send(uint32_t a_uint32counter);
void UART_send(const char *array_ptr);

#endif /* SCHEDULER_H_ */
