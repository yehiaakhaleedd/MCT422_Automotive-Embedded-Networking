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
#include "MCAL/ADC/ADC.h"
#include "HAL/led.h"
#include "HAL/buttons.h"
#include "OS/OS_config.h"
#include "MCAL/NVM/NVM.h"


/***********************************************
 * Definitions and Macros
 ***********************************************/
#define MAX_ELEMENTS     0x30U
#define CAN_TX_UNIDENTIFIED_MESSAGE_ID          0x09
#define DELAY_500MS (SysCtlClockGet() / 3 / 2)

#define NORMAL_STATE                    0x00
#define OVERHEAT                        0x01
#define FAULT_STATE                     0x02
#define UNEXPECTED_VOLTAGE_STATE        0x03

#define GPIO_ON                         0x06

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
void APP_voidOS(void);
void OS_voidCheckCANCommunication(void);
void OS_voidAddTask(void (*taskFunction)(void), uint32_t periodTicks, uint8_t priority);
void OS_voidSortTasksByPriority(void);
void OS_voidInitTasks(void);
void OS_voidInit(void);
void OS_voidMCALInit(void);
void OS_voidHALInit(void);
void OS_voidInitAll(void);
void OS_voidCANHandleReceivedMessages(void);
uint8_t OS_ui16ECU2ReadTemperature(void);
void OS_ui16SendTemperature(void);
void OS_voidECU2RespondToRemoteFrame(void);
void OS_voidECU2HandleRemoteFrame(void);
void OS_voidCheckRXOK(void);
void OS_floatCalculateAverageTemperature(void);
void OS_voidCheckState(uint8_t TempValue);
void OS_voidCheckOverheat(void);
void OS_voidHeartbeatError(void);
void OS_voidCheckVoltageAndRemote(void);
void OS_voidCheckDTC(void);
void OS_voidSendKeepAlive(void);


void INITIALIZATION_MCAL(void);
void INITIALIZATION_buttons(void);
void INITIALIZATION_leds(void);
void SPI_put(uint32_t a_uint32data);
bool SPI_check(void);

void scheduler(void);


#endif /* SCHEDULER_H_ */
