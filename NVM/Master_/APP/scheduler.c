/*
 * scheduler.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to implement a SysTick Interrupt Handler in the context of an embedded system.
 *      The file processes communication states, controls LEDs, handles UART transmission, and sends state data based on conditions.
 */


/***********************************************
 * Includes
 ***********************************************/
#include <APP/scheduler.h>
#include <HAL/initialization.h>
#include "HAL/buttons.h"
#include "HAL/led.h"



/***********************************************
 * Global and Static Variables
 ***********************************************/
bool last_sw1_state = false;
bool last_sw2_state = false;
uint32_t UartCurrentState = 0x16;
uint32_t counter = 0;
uint32_t counter_EE = 0;
static uint32_t CurrentState =0; //CHECK PLACE
static uint32_t PrevState = 0;   //CHECK PLACE

/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: SysTick_Handler
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function is the SysTick interrupt handler, executed at regular intervals.
 * - Increments the global system tick counter.
 * - Checks and handles communication state transitions.
 * - Sends the current state information over UART if communication is not lost.
 * - If communication is lost, it toggles the Red LED at a 500ms interval.
 * - Updates the previous state and current state for state machine operation.
 ***********************************************/
void SYSTICK_handler(void) {
    // Increment the millisecond counter
    g_ui32SysTickCount++;


    sw1 = (bool)BUTTONS_rightButton();
    sw2 = (bool)BUTTONS_leftButton();

    if (sw1 == true && last_sw1_state == false && sw2 == true && last_sw2_state == false) {
        //counter = STATE_INIT;
        counter_EE++;
        if(counter_EE > 1)
        {
            LED_redOff();
            counter_EE = 0;
        }

        /*SPI_masterSend(0x09);
        counter = 0;*/
    }

    else if (sw1 == true && last_sw1_state == false) {


        if(sw1 == 1 && last_sw1_state == 0 && g_ui32SysTickCount >= 60)
        {
            holdButtonFlag = true;
            LED_greenOn();
        }

        counter++;
        if (counter > 3) {
            counter = STATE_RED;
        }
    }

    else if (sw2 == true && last_sw2_state == false) {
        while ((bool)BUTTONS_leftButton() == true && (bool)BUTTONS_rightButton() == false) {} // Avoid integer/boolean mismatch
        counter--;
        if (counter < 1) {
            counter = STATE_BLUE;
        }
    }


    last_sw1_state = sw1;
    last_sw2_state = sw2;

    if(sw1 == 0)
    {
        g_ui32SysTickCount = 0;
        holdButtonFlag = false;
    }


    if(commcheck)
    {
        if(CurrentState == COMM_LOST)
        {
            counter = PrevState;
        }
        CurrentState = PrevState;
    }

    switch(counter)
    {
        case STATE_RED:
            UartState = STATE_RED;
            PrevState = STATE_RED;
            break;

        case STATE_GREEN:
            UartState = STATE_GREEN;
            PrevState = STATE_GREEN;
            break;

        case STATE_BLUE:
            UartState = STATE_BLUE;
            PrevState = STATE_BLUE;
            break;

        case STATE_INIT:
            APP_sendToPc(counter);
            counter = STATE_RED;
            break;
    }

    SPI_send(counter);


    if(!commcheck){
        counter = COMM_LOST;
        UartState = COMM_LOST;
        CurrentState = COMM_LOST;
        IncrementCounter();
    }

    if(UartState == COMM_LOST && commcheck)
    {
        recievedFlag = true;
    }

    if(holdButtonFlag)
    {
        UartState = LEAVE_BUTTON;
        PrevState = LEAVE_BUTTON;

    }

    if(whiteFlag)
    {
        UartState = STATE_WHITE;
        PrevState = STATE_WHITE;
    }
    if(SlaveButtonsFlag)
    {
        UartState = DONT_TOUCH;
        PrevState = DONT_TOUCH;
        counter = 0;
    }

    if(UartState != UartCurrentState)
    {
        if(unidentifiedFlag && !recievedFlag)
        {
            UartState = UNIDENTIFIED;
            PrevState = UNIDENTIFIED;
            APP_sendToPc(UNIDENTIFIED);
        }
        if(holdButtonFlag)
        {
            UartState = LEAVE_BUTTON;
            PrevState = LEAVE_BUTTON;
            APP_sendToPc(LEAVE_BUTTON);
            holdButtonFlag = false; // Clear the flag after sending the message
            g_ui32SysTickCount = 0; // Reset the tick counter
        }

        if(SlaveButtonsFlag)
        {
            UartState = DONT_TOUCH;
            PrevState = DONT_TOUCH;
            APP_sendToPc(DONT_TOUCH);
        }
        if(whiteFlag)
        {
            UartState = STATE_WHITE;
            PrevState = STATE_WHITE;
            APP_sendToPc(STATE_WHITE);
        }

        if(recievedFlag)
        {
            UartState = RECEIVED_COMM;
            PrevState = RECEIVED_COMM;
            APP_sendToPc(RECEIVED_COMM);
            recievedFlag = false;
        }
        if(!unidentifiedFlag  && !SlaveButtonsFlag && !whiteFlag && !holdButtonFlag)
        {
            APP_sendToPc(counter);
        }

        UartCurrentState = UartState;
    }

    if(sw1 == 0)
    {
        g_ui32SysTickCount = 0;
        holdButtonFlag = false;
    }

    if(counter == COMM_LOST)
    {
        //LED_redBlink();
    }


}

/***********************************************
 * Function Name: UartToPC
 * Inputs: uint32_t data - A value representing the state of the system.
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Based on the value of `data`, this function sends a corresponding message over UART to the PC or terminal.
 *              It checks the state and sends a specific string for each state, such as "State Machine Is Red", "State Machine Is Green",
 *              "COMMUNICATION LOST!!!!", or "UNIDENTIFIED STATE!!!". The function helps in providing real-time feedback on the system's
 *              current state or any communication issues.
 ***********************************************/

void APP_sendToPc(uint32_t a_uint32data)
{

    switch(a_uint32data)
    {
    case STATE_RED:
        UART0_send("State Machine Is Red\r\n");
        break;

    case STATE_GREEN:
        UART0_send("State Machine Is Green\r\n");
        break;

    case STATE_BLUE:
        UART0_send("State Machine Is Blue\r\n");
        break;

    case COMM_LOST:
        UART0_send("COMMUNICATION LOST!!!!\r\n");
        break;

    case UNIDENTIFIED:
        UART0_send("UNIDENTIFIED STATE!!!\r\n");
        break;

    case DONT_TOUCH:
        UART0_send("DON'T TOUCH SLAVE BUTONS!!!\r\n");
            break;

    case STATE_WHITE:
        UART0_send("Slave Is In Idle State\r\n");
            break;

    case STATE_INIT:
        UART0_send("State Machine Is Initial State\r\n");
            break;

    case RECEIVED_COMM:
        UART0_send("Communication Received\r\n");
            break;

    case LEAVE_BUTTON:
        UART0_send("Master button was pressed for too much time\r\n");
            break;
    }

}

void IncrementCounter(void) {
    static uint32_t counter = 0;

    // Step 2: Read the current counter value from EEPROM
    EEPROMRead(&counter, EEPROM_COUNTER_ADDRESS, sizeof(counter));

    if(counter > 1)
    {
        LED_redOn();
    }
    else{
        counter++;
        EEPROMProgram(&counter, EEPROM_COUNTER_ADDRESS, sizeof(counter));
    }

    // Step 3: Increment the counter






}


