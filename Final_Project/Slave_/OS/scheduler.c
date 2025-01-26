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




uint32_t APP_uint32CurrentStateIndex =0;
uint32_t APP_uint32checkcounter = 0;
uint32_t APP_uint32i=0;
uint32_t OS_delayCount=0;

uint32_t OS_ui32time1 = 100;
uint32_t OS_ui32time2 = 200;
uint32_t OS_ui32time3 = 50;
uint32_t OS_ui32time4 = 500;
uint32_t OS_ui32BlinkWhiteTimer = 0;
uint32_t OS_ui32BlinkBlueTimer = 0;
uint32_t OS_ui32AvgTimer = 0;
uint32_t OS_ui32SumOfReadings = 0;
uint32_t OS_ui32CommLostTimer = 0;

static float OS_floatTempAverage = 0;

bool  boolReturnAvgFlag = false;
bool  OS_boolCommunicationLostFlag = false;
bool  OS_boolBlinkWhiteFlag = false;
bool  OS_boolBlinkBlueFlag = false;
bool  OS_boolIncrementCommFlag = false;

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
    OS_ui32BlinkWhiteTimer++;
    OS_ui32AvgTimer++;
    OS_ui32CommLostTimer++;
    OS_ui32BlinkBlueTimer++;

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
void scheduler(void)
{
    OS_voidCheckCANCommunication();
    OS_voidCANHandleReceivedMessages();
    OS_voidCheckOverheat();
    OS_voidHeartbeatError();
    OS_voidCheckDTC();
    //OS_voidCheckVoltageAndRemote();


    //OS_ui16SendTemperature();

//    int x = getVoltage();
//
//    UART_SendMessage("Voltage: ");
//    UART_SendNumber(x);
//    UART_SendMessage("v\r\n");


    //OS_voidECU2HandleRemoteFrame();
    //OS_voidECU2RespondToRemoteFrame();
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

void OS_voidScheduler(void) {
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
    OS_voidAddTask(OS_voidCANHandleReceivedMessages , 200 , 1);
    OS_voidSortTasksByPriority();
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
        //HAL_voidLedOn((uint32_t)BLUE);
        OS_boolCommunicationLostFlag = false;
    }
    else {
//        HAL_voidLedOff((uint32_t)WHITE);
//        OS_boolCommunicationLostFlag = true;
    }

    if(OS_ui32CommLostTimer >= 10000 && !OS_boolFaultStateFlag)
    {
        if(OS_boolBlinkWhiteFlag)
        {
            HAL_voidLedOff((uint32_t)WHITE);
        }
        //HAL_voidLedOn((uint32_t)BLUE);
        OS_boolCommunicationLostFlag = true;
        OS_boolBlinkWhiteFlag = false;
        OS_boolBlinkBlueFlag = true;
        //UART_SendNumber(OS_ui32CommLostTimer);
    }

    //OS_voidSendKeepAlive();
}

void OS_voidSendKeepAlive(void) {
        uint8_t keepAliveData[1] = {0xAA}; // Example data
        CAN_SendMessage(CAN_KEEP_ALIVE_ID, CAN_KEEP_ALIVE_OBJ, keepAliveData, CAN_DATA_LENGTH);
}
void APP_voidOS(void)
{   static bool initFlag = false;

    if(!initFlag)
    {
        OS_voidInit();
        OS_voidInitTasks();
        initFlag = true;

    }else{}

//    bool systick_fire = (g_ui32SysTickCount%0x10U==0x00U);
//    if(systick_fire){
//        //OS_voidScheduler();
        scheduler();

//    }else{}
}

void OS_voidMCALInit(void)
{
    UART0_init();
    SYSTICK_init();
    CAN_Init();
    //CAN_ReceiveInit();
    CAN_ConfigureReceiveObjects();
    initADC();
    initADC1();
    initializeEEPROM();
}

void OS_voidCheckDTC(void){

    if(OS_boolFaultStateFlag || OS_boolCommunicationDTCFlag)
    {
        HAL_voidLedBlink(RED);
    }else{
    }
}
void OS_voidCheckVoltageAndRemote(void)
{
    static uint8_t KnownVoltage[2] = {0x00};

    KnownVoltage[0] = getVoltage();

    UART_SendMessage("Voltage ECU2: ");
    UART_SendNumber(KnownVoltage[0]);
    UART_SendMessage("v\r\n");

    CAN_SendMessage(CAN_REMOTE_ID, CAN_REMOTE_OBJ,KnownVoltage, CAN_DATA_LENGTH);


    //CAN_ConfigureRemoteFrameHandler(CAN_KEEP_ALIVE_OBJ,KnownVoltage);
}
void OS_voidECU2RespondToRemoteFrame(void) {
    uint8_t responseData[1] = {0x02};

    tCANMsgObject msgObject;

    // Configure the message object for response
    msgObject.ui32MsgID = CAN_REMOTE_ID;               // Same ID as the remote frame
    msgObject.ui32MsgIDMask = 0;              // No mask for response
    msgObject.ui32Flags = MSG_OBJ_REMOTE_FRAME; // Enable transmission interrupt
    msgObject.ui32MsgLen = CAN_DATA_LENGTH;                 // Two bytes for voltage
    msgObject.pui8MsgData = responseData;

    // Send the response
    CANMessageSet(CAN_BASE, CAN_REMOTE_OBJ, &msgObject, MSG_OBJ_TYPE_RX_REMOTE);

    //OS_voidCheckRXOK();

    // Check if the received message is a remote frame
    if (msgObject.ui32Flags & MSG_OBJ_REMOTE_FRAME) {
        if(msgObject.ui32MsgID == CAN_REMOTE_ID)
        {
            static uint8_t KnownVoltage[2] = {0x00};

            KnownVoltage[0] = getVoltage();

            UART_SendMessage("Voltage ECU2: ");
            UART_SendNumber(KnownVoltage[0]);
            UART_SendMessage("v\r\n");



            //HAL_voidLedOn(RED); // Indicate successful detection
            CAN_SendMessage(CAN_REMOTE_ID, CAN_REMOTE_OBJ, KnownVoltage, CAN_DATA_LENGTH);
        }

    }
}

void OS_floatCalculateAverageTemperature(void) {
    static float temperature_readings[500] = {0};  // Array to store 3 readings


    static int i = 0;  // Declare loop variable outside the loop

//    // Collect three temperature readings, spaced 500ms apart
//    for (i = 0; i <= 50; i++) {
//        temperature_readings[i] = getTemperature();  // Get temperature reading
//        //SysCtlDelay(DELAY_500MS);                   // Wait for 500ms
//        sum += temperature_readings[i];
//    }

    if(OS_ui32AvgTimer <= 500)
    {
        temperature_readings[i] = getTemperature();  // Get temperature reading
        //SysCtlDelay(DELAY_500MS);                   // Wait for 500ms
        OS_ui32SumOfReadings += temperature_readings[i];
        i++;

//        UART_SendMessage("Average Temperature: ");
//        UART_SendNumber(temperature_readings[i]);
//        UART_SendMessage("°C\r\n");
    }
    else
    {
        OS_floatTempAverage = OS_ui32SumOfReadings / i;
        i=0;
        boolReturnAvgFlag = true;
    }

//    return average;
}

void OS_voidCheckRXOK(void)
{
    uint32_t status = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);
    if (status & CAN_STATUS_RXOK) {
        HAL_voidLedOn(GREEN); // Indicate successful transmission
    }
}
void OS_voidECU2HandleRemoteFrame(void) {
    tCANMsgObject msgObject;
    uint8_t unusedBuffer[8]; // Placeholder buffer

    msgObject.pui8MsgData = unusedBuffer;

    // Check for incoming remote frame
    CANMessageGet(CAN_BASE, CAN_REMOTE_OBJ, &msgObject, true);

    // Check transmission status
    uint32_t status = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);
    if (status & CAN_STATUS_RXOK) {
        HAL_voidLedOn(GREEN); // Indicate successful transmission
    } else {
        //HAL_voidLedOn(RED); // Indicate transmission failure
    }

    if ((msgObject.ui32Flags & MSG_OBJ_REMOTE_FRAME)) {

        HAL_voidLedOn(BLUE); // Indicate successful reception
        // Respond to the remote frame
        OS_voidECU2RespondToRemoteFrame();
    }

}

// Simulate or read temperature
uint8_t OS_ui16ECU2ReadTemperature(void) {
    // Simulated temperature value (replace with ADC code)
    static uint8_t temp = 250; // Example: 25.0°C (scaled by 10)
    return temp;
}

void OS_ui16SendTemperature(void) {

    static uint8_t avg_temperature[5];
    OS_floatCalculateAverageTemperature();

    avg_temperature[0] = OS_floatTempAverage;

    if(boolReturnAvgFlag)
    {
        OS_ui32AvgTimer = 0;
        boolReturnAvgFlag = false;
        OS_ui32SumOfReadings = 0;
        CAN_SendMessage(CAN_TEMP_ID, CAN_TEMP_OBJ,avg_temperature, CAN_DATA_LENGTH); // Message Object 2, ID: 0x102
    }
    else{}


//    if(g_ui32SysTickCount % 500 == 0)
//    {
//        CAN_SendMessage(CAN_TEMP_ID, CAN_TEMP_OBJ,avg_temperature, CAN_DATA_LENGTH); // Message Object 2, ID: 0x102
//    }
//    else{}

    //OS_voidCheckRXOK();
}


// Function to handle received CAN messages
void CAN_ReceiveMessage(void) {
    static uint8_t g_ui8ReceivedData[CAN_DATA_LENGTH];

    tCANMsgObject messageObject;

    // Prepare to receive the message
    messageObject.pui8MsgData = g_ui8ReceivedData;

    // Get the received message
    CANMessageGet(CAN_BASE, (uint32_t)CAN_RX_OBJECT_NUM, &messageObject, true);

}

void OS_voidCANHandleReceivedMessages(void) {
    uint8_t receivedData[8];
    tCANMsgObject msgObject;

    // Temporary storage for message data
    msgObject.pui8MsgData = receivedData;

    // Check Message Object 2 (Temperature data)
    CANMessageGet(CAN_BASE, CAN_KEEP_ALIVE_OBJ, &msgObject, true);


    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {

        if(msgObject.ui32MsgID == CAN_REMOTE_ID)
        {
            OS_voidCheckVoltageAndRemote();
        }

        if(msgObject.ui32MsgID == CAN_KEEP_ALIVE_ID)
        {
            OS_ui16SendTemperature();
            //OS_voidSendKeepAlive();
            //HAL_voidLedOn(BLUE);
            //UART_SendNumber(receivedData[0]);
            OS_ui32CommLostTimer = 0;
            OS_boolBlinkBlueFlag = false;
            OS_boolFaultStateFlag = false;
            OS_boolIncrementCommFlag = false;
            NVM_CommRet();
        }
        else{
        }
        if(msgObject.ui32MsgID == CAN_STATE_ID)
        {
            //HAL_voidLedBlink(GREEN);
            OS_voidCheckState(receivedData[0]);
            //UART_SendNumber(receivedData[0]);
        }
        if(msgObject.ui32MsgID == CAN_GPIO_CONTROL_ID)
        {
            HAL_voidLedOn(RED);

        }else{

        }

    }

}

void OS_voidCheckState(uint8_t STATE)
{
    if(STATE == NORMAL_STATE && !OS_boolBlinkWhiteFlag)
    {
        if(!OS_boolBlinkWhiteFlag)
        {
            HAL_voidLedOff(WHITE);
        }
        HAL_voidLedOn(GREEN);
    }
    else if(STATE == OVERHEAT)
    {
        HAL_voidLedOff(GREEN);
        OS_boolBlinkWhiteFlag = true;
        //OS_voidECU2RespondToRemoteFrame();
    }
    else if(STATE == FAULT_STATE)
        {
            HAL_voidLedOff(GREEN);
            OS_boolFaultStateFlag = true;
            NVM_StateReminder();
        }
    else if(STATE == UNEXPECTED_VOLTAGE_STATE)
    {
        HAL_voidLedOff(GREEN);
        OS_boolFaultStateFlag = true;
        NVM_StateReminder();

    }

//    if(OS_boolFaultStateFlag)
//    {
//        HAL_voidLedBlink(RED);
//    }
}

void OS_voidCheckOverheat(void)
{

    if(OS_boolBlinkWhiteFlag && !OS_boolCommunicationLostFlag && !OS_boolFaultStateFlag)
    {
        if(OS_ui32BlinkWhiteTimer >= 20000)
        {
            OS_boolBlinkWhiteFlag = false;
        }
        HAL_voidLedBlink(WHITE);
    }
    else if(!OS_boolBlinkWhiteFlag && !OS_boolFaultStateFlag){
        //HAL_voidLedOff(WHITE);
        OS_ui32BlinkWhiteTimer = 0;
    }

}

void OS_voidHeartbeatError(void)
{
    if(OS_boolBlinkBlueFlag && !OS_boolFaultStateFlag && !OS_boolCommunicationDTCFlag)
    {
        if(!OS_boolIncrementCommFlag){
            OS_boolIncrementCommFlag = true;
            NVM_voidIncrementCommunicationDTCCounter();
        }

        if(OS_ui32BlinkBlueTimer >= 20000)
        {
            HAL_voidLedOff(BLUE);
            OS_boolBlinkBlueFlag = false;
        }
        HAL_voidLedOff(GREEN);
        HAL_voidLedBlink(BLUE);
    }
    else
    {
        HAL_voidLedOff(BLUE);
        OS_ui32BlinkBlueTimer = 0;
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
