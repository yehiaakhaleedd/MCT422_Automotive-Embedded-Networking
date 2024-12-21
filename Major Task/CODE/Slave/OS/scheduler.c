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
//#include "HAL/Initialization.h"


/***********************************************
 * Global and Static Variables
 ***********************************************/
uint32_t PrevState = 0x90;

bool APP_boolWhiteFlag = false;
bool APP_boolWhiteStateFlag = false;
bool boolUnidentifiedFlag = false;
bool sw1; // Right button
bool holdButtonFlag = false;

uint32_t APP_uint32CurrentStateIndex =0;
uint32_t APP_uint32checkcounter = 0;
uint32_t APP_uint32i=0;
uint32_t OS_delayCount=0;

uint32_t OS_ui32time1 = 100;
uint32_t OS_ui32time2 = 200;
uint32_t OS_ui32time3 = 50;
uint32_t OS_ui32time4 = 500;



uint8_t UNIDENTIFIED = 0x30;
uint8_t ui8taskCount = 0;


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
    OS_delayCount++;

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
    OS_voidSchedulerr();


}

void APP_ui32CheckButtons(void)
{
    sw1 = (bool)BUTTONS_rightButton();

    bool is_sw1 = (sw1 == 0x01U);
    if(is_sw1)
    {
        holdButtonFlag = true;
    }else
    {
        holdButtonFlag = false;
    }

}

void OS_voidTurnOnLED(void)
{
    if(holdButtonFlag)
    {
        LED_whiteOn();
    }
    else{
        LED_whiteOff();
    }
}
void OS_voidCheckErrors(void)
{
    OS_voidCheckUnidentified();
    OS_voidCheckSlaveButtons();
}

void OS_voidCheckSlaveButtons(void)
{
    bool is_left = (BUTTONS_leftButton()==0x01U);
    bool is_right = (BUTTONS_rightButton()==0x01U);
    if(is_left || is_right)
    {
        uint8_t DONT_TOUCH = 0x40;
        CAN_Send(CAN_TX_MESSAGE_ID,&DONT_TOUCH,CAN_DATA_LENGTH);
        //HAL_voidLedOn(WHITE);
    }
    else{}
}
void OS_voidCheckUnidentified(void)
{
    if(boolUnidentifiedFlag)
    {

        CAN_Send(CAN_TX_MESSAGE_ID,&UNIDENTIFIED,CAN_DATA_LENGTH);
        boolUnidentifiedFlag = false;
        //LED_whiteOn();
    }

}

void OS_voidAddTask(void (*taskFunction)(void), uint32_t periodTicks, uint8_t priority) {

    bool check_list = (ui8taskCount < MAX_TASKS);
    if (check_list) {
        taskList[ui8taskCount].taskFunction = taskFunction;
        taskList[ui8taskCount].periodTicks = periodTicks;
        taskList[ui8taskCount].lastRunTick = 0x00U;  // Initialize last run to 0
        taskList[ui8taskCount].priority = priority;
        taskList[ui8taskCount].isEnabled = 0x01U;    // Enable by default
        taskList[ui8taskCount].state = TASK_READY; // Set state to READY
        ui8taskCount++;
    }
}

void OS_voidSortTasksByPriority(void) {
    uint8_t i = 0;
    uint8_t j = 0;
    for (i = 0; i < ui8taskCount - 1; i++) {
        for (j = i + 1; j < ui8taskCount; j++) {
            if (taskList[i].priority > taskList[j].priority) {
                TaskControlBlock temp = taskList[i];
                taskList[i] = taskList[j];
                taskList[j] = temp;
            }
        }
    }
}

void OS_voidSchedulerr(void) {
    uint32_t currentTick = g_ui32SysTickCount;
    uint8_t highestPriorityIndex = MAX_TASKS;
    uint8_t i = 0;

    bool check_en = (taskList[i].isEnabled);
    bool check_task = (taskList[i].state == TASK_READY);
    bool check_priority = (highestPriorityIndex == MAX_TASKS);
    bool list =(taskList[i].priority < taskList[highestPriorityIndex].priority);
    for ( i = 0; i < ui8taskCount; i++) {
        if (check_en && check_task) {
            // Check if the task is ready to run
            if ((currentTick - taskList[i].lastRunTick) >= taskList[i].periodTicks) {
                if (check_priority || list) {
                    highestPriorityIndex = i;
                }
            }
        }
    }

    // Execute the highest-priority task, if any
    if (check_priority) {
        taskList[highestPriorityIndex].state = TASK_RUNNING;
        taskList[highestPriorityIndex].taskFunction();
        taskList[highestPriorityIndex].lastRunTick = currentTick;
        taskList[highestPriorityIndex].state = TASK_READY;
    } else {
        // Run idle task if no tasks are ready
        //OS_IdleTask();
    }

    // If no tasks are ready, run the idle task
    //OS_IdleTask();
}

void OS_voidInitTasks(void) {
    OS_voidAddTask(&CAN_ReceiveMessage, OS_ui32time1, (uint8_t)1);     // Runs every 100ms
    OS_voidAddTask(&OS_voidQueueControl, OS_ui32time2, (uint8_t)2);    // Runs every 200ms
    OS_voidAddTask(&OS_voidCheckErrors, OS_ui32time3, (uint8_t)(uint8_t)3);           // Runs every 50ms
    OS_voidAddTask(&OS_voidCheckCANCommunication, OS_ui32time3, (uint8_t)3); // Runs every 50ms
    OS_voidAddTask(&OS_voidCheckIdle, OS_ui32time4, (uint8_t)4);        // Runs every 500ms
    OS_voidAddTask(&OS_voidStateControll, OS_ui32time4, (uint8_t)5);           // Runs every 1000ms

    OS_voidSortTasksByPriority();
}

void OS_voidStateControll(void)
{
    OS_voidStateControl(counter);
}
void OS_voidQueueControl(void)
{
    if(CurrentState != PrevState)
    {

        bool check_index = (APP_uint32CurrentStateIndex < MAX_ELEMENTS);
        if(check_index)
        {
            StoredValues[APP_uint32CurrentStateIndex] = CurrentState;
            APP_uint32CurrentStateIndex++;
            counter = CurrentState;
        }
    }
}

void OS_voidCheckIdle(void)
{
    if(APP_boolWhiteStateFlag)
    {
        APP_boolWhiteStateFlag = false;
    }else{}

    bool check_idle = (APP_boolWhiteFlag);
    bool check_comm = (counter != COMM_LOST);
    if(check_idle && check_comm)
    {
        counter = STATE_WHITE;

        APP_boolWhiteFlag = false;
    }else{}


    if(counter == STATE_WHITE)
    {
        HAL_voidLedOn((uint32_t)WHITE);
        APP_boolWhiteStateFlag = true;
    }else{}
}


void OS_voidCheckCANCommunication(void) {
    uint32_t ui32CANStatus;

    // Get the current status of the CAN controller
    ui32CANStatus = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);

    // Check for an acknowledgment error
    if (ui32CANStatus & CAN_STATUS_LEC_ACK) {
        HAL_voidLedOff((uint32_t)WHITE);
    }
    // Check for successful transmission
    else if (ui32CANStatus & (uint32_t)CAN_STATUS_RXOK) {
        //LED_whiteOn();
    }
    else {
        // No specific error, CAN is operating normally
        //UARTprintf("CAN Communication OK\n");

    }


}

void OS_voidStateControl(uint32_t a_ui32counter)
{
    bool a = (APP_uint32i<APP_uint32CurrentStateIndex);
    bool b = (OS_delayCount>=(uint32_t)1000);
    while(a && b)
    {
        counter = StoredValues[APP_uint32i];

        if(CurrentState != PrevState)
        {
            PrevState = CurrentState;
            APP_boolWhiteFlag = false;
            switch(a_ui32counter)
            {
            case STATE_RED :
                HAL_voidLedOff((uint32_t)BLUE);
                HAL_voidLedOff((uint32_t)GREEN);
                HAL_voidLedOn((uint32_t)RED);
                break;

            case STATE_GREEN:
                HAL_voidLedOff((uint32_t)BLUE);
                HAL_voidLedOn((uint32_t)GREEN);
                HAL_voidLedOff((uint32_t)RED);
                break;

            case STATE_BLUE:
                HAL_voidLedOn((uint32_t)BLUE);
                HAL_voidLedOff((uint32_t)GREEN);
                HAL_voidLedOff((uint32_t)RED);
                break;

            case COMM_LOST:
                APP_boolWhiteFlag = false;
                HAL_voidLedOff((uint32_t)WHITE);
                break;

            case STATE_WHITE:
                HAL_voidLedOn((uint32_t)WHITE);
                break;
            }
            APP_uint32i++;

        }

        APP_uint32i=0x00U;
        OS_delayCount = 0x00U;
        APP_uint32CurrentStateIndex = 0x00U;
    }
}

void APP_voidOS(void)
{   static bool initFlag = false;

    if(!initFlag)
    {
        OS_voidInit();
        OS_voidInitTasks();
        initFlag = true;

    }else{}

    bool systick_fire = (g_ui32SysTickCount%0x10U==0x00U);
    if(systick_fire){
        Scheduler();
    }else{}
}

void OS_voidMCALInit(void)
{
    UART0_init();
    SYSTICK_init();
    CAN_Init();
    CAN_ReceiveInit();
}



// Function to handle received CAN messages
void CAN_ReceiveMessage(void) {
    static uint8_t g_ui8ReceivedData[CAN_DATA_LENGTH];

    tCANMsgObject messageObject;

    // Prepare to receive the message
    messageObject.pui8MsgData = g_ui8ReceivedData;


    bool state_red = (g_ui8ReceivedData[0] == STATE_RED);
    bool state_green = (g_ui8ReceivedData[0] == STATE_GREEN);
    bool state_blue = (g_ui8ReceivedData[0] == STATE_BLUE);
    bool state_white = (g_ui8ReceivedData[0] == STATE_WHITE);
    bool state_init = (g_ui8ReceivedData[0] == STATE_INIT);
    bool state_zero = (g_ui8ReceivedData[0] == 0x00U);
    // Get the received message
    CANMessageGet(CAN_BASE, (uint32_t)CAN_RX_OBJECT_NUM, &messageObject, true);
    if(state_red || state_green || state_blue || state_white || state_init || state_zero)
    {
        CurrentState = g_ui8ReceivedData[0];
    }
    if(g_ui8ReceivedData[0] == UNIDENTIFIED)
    {
        boolUnidentifiedFlag = true;
        LED_whiteOff();
    }else{}

    if(CurrentState == STATE_WHITE)
    {
        LED_whiteOn();
    }

}


void OS_voidInit(void)
{
    OS_voidMCALInit();

    OS_voidHALInit();


}

void OS_voidHALInit(void)
{

    INITIALIZATION_buttons();
    INITIALIZATION_leds();

}


/***********************************************
 * Function Name: INITIALIZATION_buttons
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the buttons by calling the PortF_Init function.
 * This function configures the GPIO pins for button inputs on Port F.
 ***********************************************/
void INITIALIZATION_buttons(void)
{
    BUTTONS_initPortF();
}


/***********************************************
 * Function Name: INITIALIZATION_leds
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the LEDs by calling the LED_initPortF function
 *              to configure the necessary GPIO pins for LED control.
 ***********************************************/
void INITIALIZATION_leds(void)
{
    LED_initPortF();
}
