/*
 * buttons.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Define functions for detecting button states (e.g., pressed or released).
 *               2) Provide a function to initialize the GPIO port and pins associated with the buttons.
 *               3) Abstract low-level GPIO operations for easier use in application code.
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_


/***********************************************
 * Includes
 ***********************************************/
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
 * Type Declarations (enums, structs and unions)
 ***********************************************/


/***********************************************
 * Functions Prototypes
 ***********************************************/
void PortF_Init(void);
void BUTTONS_initPortF(void);
bool BUTTONS_rightButton(void);
bool BUTTONS_leftButton(void);

#endif /* BUTTONS_H_ */
