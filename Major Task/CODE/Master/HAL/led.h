/*
 * led.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Facilitate SPI initialization and communication for both master and slave roles.
 *               2) Abstract low-level hardware-specific operations related to SPI, simplifying the implementation
 *               3) Enable data exchange between devices, such as a microcontroller (master) and peripheral devices (slave).
 *               4) Provide status flags and acknowledgment mechanisms to ensure reliable communication
 */

#ifndef LED_H_
#define LED_H_


/***********************************************
 * Includes
 ***********************************************/
#include <MCAL/Timers/TIMER0/timer0.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "inc/hw_gpio.h"
#include "OS/scheduler.h"

/***********************************************
 * Definitions and Macros
 ***********************************************/
#define RED       0x00U
#define GREEN     0x01U
#define BLUE      0x02U
#define WHITE     0x03U




/***********************************************
 * Functions Prototypes
 ***********************************************/
void LED_initPortF(void);
void LED_blueOn(void);
void LED_blueOff(void);
void LED_greenOn(void);
void LED_greenOff(void);
void LED_redOn(void);
void LED_redOff(void);
void LED_whiteOn(void);
void LED_whiteOff(void);
void LED_redBlink(void);
void HAL_voidLedOn(uint32_t LedState);
void HAL_voidLedOff(uint32_t LedState);
void HAL_voidLedBlink(uint32_t LedState);

#endif /* LED_H_ */
