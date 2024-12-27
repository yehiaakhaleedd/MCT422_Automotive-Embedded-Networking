/*
 * scheduler.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to implement a SysTick Interrupt Handler in the context of an embedded system.
 *               This file acts as a scheduler and state manager, integrating data from multiple sources (SPI, buttons, and timers)
 *               to control the behavior of the system. Its primary responsibilities include:
 *
 *               - Processing SPI data to determine state transitions.
 *               - Managing LEDs to reflect the current state.
 *               - Handling timeouts and communication loss to ensure system robustness.
 *               - Prioritizing button inputs over SPI commands for manual state changes.
 *               - Coordinating system timing and operations via interrupts.
 */

/***********************************************
 * Includes
 ***********************************************/
#include "scheduler.h"
#include "HAL/Initialization.h"


/***********************************************
 * Global and Static Variables
 ***********************************************/
uint32_t PrevState = 0x90;

bool APP_boolWhiteFlag = false;
bool APP_boolWhiteStateFlag = false;

uint32_t APP_uint32CurrentStateIndex =0;
uint32_t APP_uint32checkcounter = 0;
uint32_t APP_uint32i=0;

/***********************************************
 * Function Name: SYSTICK_handler
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: The function handles the SysTick interrupt, which triggers every
 *              millisecond. It increments the global `g_ui32SysTickCount` and
 *              checks if data has been received via SPI. Depending on the received
 *              data and the state of certain buttons, it sends different responses
 *              via SPI (e.g., `UNIDENTIFIED`, `DONT_TOUCH`, `STATE_WHITE`, or `ACK`).
 *              If the received state differs from the previous state, it updates the
 *              stored values and the `counter` variable. Additionally, it monitors
 *              the timeout condition using `APP_uint32checkcounter` and handles
 *              the transition to `COMM_LOST` if necessary. If the `APP_boolWhiteFlag`
 *              is set, it changes the `counter` to `STATE_WHITE`.
 ***********************************************/
void SYSTICK_handler(void) {
    // Increment the millisecond counter
    g_ui32SysTickCount++;

    if (SPI_check())
    {
        if(receivedData > 0x04)
        {
            SPI_put(UNIDENTIFIED);
        }
        else if(BUTTONS_leftButton()==1 || BUTTONS_rightButton()==1)
        {
            SPI_put(DONT_TOUCH);
        }
        else if(APP_boolWhiteStateFlag)
        {
            SPI_put(STATE_WHITE);
            APP_boolWhiteStateFlag = false;
        }
        else
            SPI_put(ACK);

        CurrentState = receivedData;

        if(CurrentState != PrevState)
        {
            if(APP_uint32CurrentStateIndex < MAX_ELEMENTS)
            {
                StoredValues[APP_uint32CurrentStateIndex] = receivedData;
                APP_uint32CurrentStateIndex++;
                counter = receivedData;
            }
        }

        APP_uint32checkcounter = 0;
    }
    else
        APP_uint32checkcounter++;

    if(APP_uint32checkcounter > 20)
    {
        LED_whiteOff();
        counter = COMM_LOST;
        APP_uint32checkcounter = 0;
    }
    if(APP_boolWhiteFlag && counter != COMM_LOST)
    {
        counter = STATE_WHITE;
        APP_boolWhiteFlag = false;

    }
}


/***********************************************
 * Function Name: Scheduler
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: The function manages the state transitions and controls
 *              the operation of LEDs based on the current state. It checks the
 *              `counter` variable to determine which state to set the LEDs to
 *              (Red, Green, Blue, White, or COMM_LOST). The function also
 *              handles the initialization of Timer1 and manages flags, including
 *              `APP_boolWhiteFlag` and `APP_boolWhiteStateFlag`. The state transitions
 *              occur in a loop, and after each state update, it introduces a delay of
 *              1 second. After the loop, it resets the index to begin the cycle again.
 ***********************************************/
void Scheduler(void)
{
    if(counter == STATE_WHITE)
    {
        LED_whiteOn();
        APP_boolWhiteStateFlag = true;
        return;
    }

    while(APP_uint32i<APP_uint32CurrentStateIndex)
    {
        counter = StoredValues[APP_uint32i];

        if(CurrentState != PrevState)
        {
            PrevState = CurrentState;
            APP_boolWhiteFlag = false;
            g_ui32timer1Count = 0;
            switch(counter)
            {
            case STATE_RED :
                LED_blueOff();
                LED_greenOff();
                LED_redOn();
                INITIALIZATION_startTimer1();
                break;

            case STATE_GREEN:
                LED_blueOff();
                LED_greenOn();
                LED_redOff();
                INITIALIZATION_startTimer1();
                break;

            case STATE_BLUE:
                LED_blueOn();
                LED_greenOff();
                LED_redOff();
                INITIALIZATION_startTimer1();
                break;

            case COMM_LOST:
                APP_boolWhiteFlag = false;
                LED_whiteOff();
//                IncrementCounter();
                break;

            case STATE_WHITE:
                LED_whiteOn();
                break;
            }

            TIMER0_delayMSec(1000);
            APP_uint32i++;
        }

        APP_uint32i=0;

        APP_uint32CurrentStateIndex = 0;
    }

}




/***********************************************
 * Function Name: TIMER1_handler
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Interrupt handler for Timer1. This function is called when
 *              Timer1 reaches its timeout. It clears the interrupt flag and
 *              increments the global timer count (`g_ui32timer1Count`). If
 *              the timer count reaches 10 and the `CurrentState` is the same
 *              as the `PrevState`, it sets the `APP_boolWhiteFlag` to true.
 ***********************************************/
void TIMER1_handler(void) {
    // Clear the interrupt flag
    TIMER1_clearInt();
    g_ui32timer1Count++;

    if(g_ui32timer1Count >= 10 && CurrentState == PrevState)
    {
        APP_boolWhiteFlag = true;
    }
}

void IncrementCounter(void) {
    static uint32_t counter = 0;

    // Step 1: Initialize the EEPROM
    if (EEPROMInit() != EEPROM_INIT_OK) {
        // Handle initialization failure
        while (1);
    }

    // Step 2: Read the current counter value from EEPROM
    EEPROMRead(&counter, EEPROM_COUNTER_ADDRESS, sizeof(counter));

    // Step 3: Increment the counter
    counter++;

    // Step 4: Write the updated counter value back to EEPROM
    uint32_t status = EEPROMProgram(&counter, EEPROM_COUNTER_ADDRESS, sizeof(counter));


    if(status > 1 )
    {
        LED_redOn();
    }
    else{}
}

