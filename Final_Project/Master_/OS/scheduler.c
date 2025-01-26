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
#include <OS/scheduler.h>

/***********************************************
 * Global and Static Variables
 ***********************************************/
uint32_t APP_ui32UartCurrentState = 0x16;
uint8_t APP_ui32counter = 0;
uint8_t ui8taskCount = 0;
uint32_t APP_ui32idleCounter = 0;
uint32_t OS_ui32CurrentError = 0;
uint32_t OS_ui32PrevError = 0x12;
uint32_t OS_ui32index = 0;
uint32_t OS_ui32DTCTimer = 0;
uint32_t OS_ui32BlinkTimer = 0;
uint32_t OS_ui32TesterTimer = 0;
uint32_t OS_ui32CommLostTimer = 0;
uint32_t OS_ui32BlinkBlueTimer = 0;
uint32_t OS_ui32CommFailure = 0;

static uint32_t APP_ui32CurrentState =0; //CHECK PLACE
static uint32_t APP_ui32PrevState = 0;   //CHECK PLACE
static uint8_t g_ui8ReceivedData[CAN_DATA_LENGTH] = {0};
//static uint8_t OS_ui8OverheatDTCCounter = 0;

bool APP_boolStateInit = false;
bool OS_boolidleFlag = false;
bool OS_boolBlinkWhiteFlag = false;
bool OS_boolBlinkBlueFlag = false;
bool OS_boolIncrementCommFlag = false;
bool OS_boolReadSensorFlag = false;
//bool OS_boolDTCFlag = false;
/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: SYSTICK_handler
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Reentrant
 * Synchronous: Synch
 * Description: This is the SysTick interrupt handler. It is triggered at regular
 *              intervals based on the configured SysTick timer settings. The
 *              function increments global counters used for timekeeping, idle
 *              state monitoring, and tracking the duration for which specific
 *              buttons are held down. These counters are critical for state
 *              management and timing operations across the system.
 ***********************************************/
void SYSTICK_handler(void) {
    // Increment the global system tick counter (used for timekeeping)
    g_ui32SysTickCount++;
    OS_ui32DTCTimer++;
    OS_ui32BlinkTimer++;
    OS_ui32TesterTimer++;
    OS_ui32CommLostTimer++;
    OS_ui32BlinkBlueTimer++;
    OS_ui32CommFailure++;

}

/***********************************************
 * Function Name: APP_voidOS
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function acts as the operating system (OS) task manager.
 *              It ensures that the initialization of the OS occurs only once
 *              and periodically calls the `APP_voidScheduler` function based
 *              on the system tick count. The scheduler is executed every 50
 *              milliseconds.
 ***********************************************/
void APP_voidOS(void) {

    static bool initFlag = false;

    // Perform initialization only once
    if (!initFlag) {
        OS_voidInit();  // Initialize the OS
        OS_voidInitTasks();

        initFlag = true; // Mark initialization as complete
    } else {
        // No action required after initialization
    }

    // Check if 50 milliseconds have passed based on the system tick
//    if (g_ui32SysTickCount % 50 == 0) {
        //APP_voidScheduler(); // Call the scheduler function
        scheduler();
//    } else {
//
//    }
}

void scheduler(void)
{
    OS_voidTesterMode();
    OS_voidCheckDTC();
    OS_voidECU1SendKeepAlive();
    OS_voidCANHandleReceivedMessages();
    OS_voidCheckOverheat();
    OS_voidHeartbeatError();
    APP_voidUartControl();




//    HAL_voidLedBlink(WHITE);


//    OS_voidECU1ReceiveVoltageResponse();
}

void OS_voidTesterMode(void)
{
    if (BUTTONS_rightButton() && BUTTONS_leftButton()) {
        // If sw1 is pressed, increment the counter and reset idle counter
        UART_SendMessage("Entering Tester Mode...\r\n");
        OS_voidEnterTesterMode();  // Enter Tester Mode
        }
    else{}
}
void OS_voidECU1SendRemoteFrame(void) {
    tCANMsgObject msgObject;

    // Configure the message object for remote frame
    msgObject.ui32MsgID = CAN_REMOTE_ID;              // ID for the known voltage request
    msgObject.ui32MsgIDMask = 0;             // No mask for transmission
    msgObject.ui32Flags = MSG_OBJ_REMOTE_FRAME; // Remote frame flag
    msgObject.ui32MsgLen = 0;                // No data for a remote request

    // Send the remote frame
    CANMessageSet(CAN_BASE, CAN_REMOTE_OBJ, &msgObject, MSG_OBJ_TYPE_TX_REMOTE);

//    // Check transmission status
//    uint32_t status = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);
//    if (status & CAN_STATUS_TXOK) {
//        HAL_voidLedOn(GREEN); // Indicate successful transmission
//    }

}

void OS_voidCANHandleReceivedMessages(void) {
    uint8_t receivedData[8];
    tCANMsgObject msgObject;

    // Temporary storage for message data
    msgObject.pui8MsgData = receivedData;

    // Check Message Object 2 (Temperature data)
    CANMessageGet(CAN_BASE, CAN_TEMP_OBJ, &msgObject, true);


    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {

        if(msgObject.ui32MsgID == CAN_REMOTE_ID)
        {
//            HAL_voidLedOn(GREEN);
//            UART_SendNumber(receivedData[0]);

            OS_voidCheckKnownVoltage(receivedData[0]);

            UART_SendMessage("Voltage ECU1: ");
            UART_SendNumber(receivedData[0]);
            UART_SendMessage("v\r\n");
        }

         if(msgObject.ui32MsgID == CAN_KEEP_ALIVE_ID){
//             UART_SendNumber(receivedData[0]);
//             OS_ui32CommLostTimer = 0;

        }
         if (msgObject.ui32MsgID == CAN_TEMP_ID)
        {
            static uint8_t OS_ui8TempValue;
            OS_ui8TempValue = receivedData[0];
            OS_voidTempData(OS_ui8TempValue);
            OS_ui32CommLostTimer = 0;
            OS_boolBlinkBlueFlag = false;
            OS_boolIncrementCommFlag = false;
            OS_ui32CommFailure = 0;
            //OS_voidCheckNormalCondition(OS_ui8TempValue);

            //HAL_voidLedOn(RED);
//            UART_SendMessage("Average Temperature: ");
//            UART_SendNumber(receivedData[0]);
//            UART_SendMessage("°C\r\n");

        }

    }


//    // Check Message Object 3 (Diagnostic queries)
//    CANMessageGet(CAN_BASE, CAN_KEEP_ALIVE_OBJ, &msgObject, true);
//    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {
//        if(msgObject.ui32MsgID == CAN_REMOTE_ID)
//        {
//            HAL_voidLedOn(GREEN);
//            UART_SendNumber(receivedData[0]);
//        }
//    }

//    // Check Message Object 3 (Diagnostic queries)
//    CANMessageGet(CAN_BASE, CAN_REMOTE_ID, &msgObject, true);
//    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {
//        HAL_voidLedOn(RED);
//        UART_SendNumber(receivedData[0]);
//    }
//
//    // Check Message Object 3 (Diagnostic queries)
//    CANMessageGet(CAN_BASE, 3, &msgObject, true);
//    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {
//        HAL_voidLedOn(RED);
//        // Handle diagnostic query
//        //ProcessDiagnosticQuery(receivedData);
//    }
//)
//    CANMessageGet(CAN_BASE, CAN_KEEP_ALIVE_OBJ, &msgObject, true);
//    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {
//        HAL_voidLedOn(RED);
//        UART_SendNumber(receivedData[0]);
//    }

}
void OS_voidCheckKnownVoltage(uint8_t Voltage)
{
    static uint8_t Overheat[] = {OVERHEAT};
    if(Voltage == 3){
        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, Overheat, CAN_DATA_LENGTH);
        NVM_voidIncrementDTCCounter();
        OS_boolBlinkWhiteFlag = true;
        //UART_send("OVERHEAT!!\r\n");
        UartState = OVERHEAT;
    }
    else if(Voltage < 3)
    {
        //UART_send("Sensor Damaged!!\r\n");
        UartState = SENSOR_DAMAGED;
        g_DTC = 2;  // Set the DTC value
        EEPROMProgram(&g_DTC, EEPROM_DTC_ADDR, sizeof(g_DTC));  // Save DTC to EEPROM
        OS_boolVoltageDTCFlag = true;
    }
}
void OS_voidCheckNormalCondition(uint8_t TempValue)
{
    static uint8_t NormalCondition[] = {0x00};
    static uint8_t Overheat[] = {0x01};

    if(TempValue <= 25)
    {
        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, NormalCondition, CAN_DATA_LENGTH);
    }
    else
    {
        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, Overheat, CAN_DATA_LENGTH);
    }
}

void OS_voidCheckOverheat(void)
{

    if(OS_boolBlinkWhiteFlag)
    {
        if(OS_ui32BlinkTimer >= 20000 && !OS_boolDTCFlag && !OS_boolVoltageDTCFlag && !OS_boolBlinkBlueFlag)
        {
            OS_boolBlinkWhiteFlag = false;
        }
        UartState = OVERHEAT;
        HAL_voidLedBlink(WHITE);
    }
    else if(!OS_boolBlinkWhiteFlag && !OS_boolDTCFlag && !OS_boolVoltageDTCFlag && !OS_boolBlinkBlueFlag){
        HAL_voidLedOff(WHITE);
        OS_ui32BlinkTimer = 0;
    }


}

void OS_voidTempData(uint8_t TempValue)
{
    uint8_t OS_ui8OverheatThreshold = 25;
    static uint8_t NormalCondition[] = {NORMAL_STATE};



    if(TempValue > OS_ui8OverheatThreshold)
    {
        if(OS_boolBlinkWhiteFlag)
        {
            OS_ui32DTCTimer = 0;
        }

        if(OS_ui32DTCTimer >= 3000)
        {
            OS_voidECU1SendRemoteFrame();
            //OS_ui8OverheatDTCCounter++;

//            CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, Overheat, CAN_DATA_LENGTH);
//            NVM_voidIncrementDTCCounter();
//            OS_boolBlinkWhiteFlag = true;
//            UART_send("OVERHEAT!!\r\n");
        }
    }
    else
    {
        UartState = NORMAL_STATE;

        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, NormalCondition, CAN_DATA_LENGTH);
        OS_ui32DTCTimer = 0;
    }


//    if(OS_ui8OverheatDTCCounter >= 3)
//    {
//        OS_boolDTCFlag = true;
//        OS_boolBlinkWhiteFlag = false;
//        HAL_voidLedOn(RED);
//    }

}

void OS_voidCheckDTC(void)
{
    static uint8_t FaultState[] = {FAULT_STATE};
    static uint8_t UnexpectedVoltage[] = {UNEXPECTED_VOLTAGE_STATE};
    static uint8_t CommunicationLost[] = {COMMUNICATION_LOST_STATE};
    static uint8_t ui8faultcounter = 0;
    static uint8_t ui8voltagecounter = 0;
    static uint8_t ui8communicationcounter = 0;


    if(OS_boolDTCFlag){
        UartState = FAULT_STATE;
        //UART_send("Fault State\r\n");
        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, FaultState, CAN_DATA_LENGTH);
        HAL_voidLedOff(GREEN);
        HAL_voidLedBlink(RED);
        OS_boolBlinkWhiteFlag = false;

        ui8faultcounter++;
        if(ui8faultcounter >= 40){
            CANDisable(CAN_BASE);
            //ui8counter = 0;
        }
    }
    else if(OS_boolVoltageDTCFlag){
        UartState = SENSOR_DAMAGED;
        //UART_send("Unexpected voltage\r\n");
        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, UnexpectedVoltage, CAN_DATA_LENGTH);
        HAL_voidLedBlink(RED);
        OS_boolBlinkWhiteFlag = false;

        ui8voltagecounter++;
        if(ui8voltagecounter >= 100){
            CANDisable(CAN_BASE);
        }
    }
    else if(OS_boolCommunicationDTCFlag){
        UartState = COMMUNICATION_LOST_STATE;
        //UART_send("Communication lost state\r\n");
        CAN_SendMessage(CAN_STATE_ID, CAN_STATE_OBJ, CommunicationLost, CAN_DATA_LENGTH);
        HAL_voidLedBlink(RED);
        OS_boolBlinkWhiteFlag = false;

        ui8communicationcounter++;
        if(ui8communicationcounter >= 100){
            CANDisable(CAN_BASE);
        }
    }
    else{
        ui8faultcounter = 0;
        ui8voltagecounter = 0;
        ui8communicationcounter = 0;
        CANEnable(CAN_BASE);
    }
}
void processTesterCommand(uint32_t command) {

    switch (command) {
    case CMD_READ_DTC:{
        UART_SendMessage("Reading DTC...\r\n");
        readDTC();
        break;
    }
    case CMD_CLEAR_DTC:{
        UART_SendMessage("Clearing DTC...\r\n");
        OS_boolDTCFlag = false;
        OS_boolVoltageDTCFlag = false;
        OS_boolCommunicationDTCFlag = false;
        CANEnable(CAN_BASE);
        clearDTC();
        break;
    }

    case CMD_READ_SENSOR: {
        UART_SendMessage("Reading Sensor Data...\r\n");

        OS_boolReadSensorFlag = true;

//        uint8_t OS_ui8count = 0;
//        if(OS_ui8count>=1000){
//        UART_SendMessage("Average Temperature: ");
//        UART_SendNumber(OS_ui8TempValue);
//        UART_SendMessage("°C\r\n");
//        }
//        OS_ui8count++;

        break;
    }

    case CMD_TEST_GPIO_ECU2:{
        UART_SendMessage("Testing GPIO on ECU 2...\r\n");

        // Example: Send GPIO toggle command to ECU 2
        uint8_t gpioCommand[1] = {GPIO_ON};  // Toggle GPIO
        CAN_SendMessage(CAN_GPIO_CONTROL_ID, CAN_GPIO_CONTROL_OBJ, gpioCommand, CAN_DATA_LENGTH);
        break;
    }

    case CMD_TEST_GPIO_ECU1:{
        UART_SendMessage("Testing GPIO on ECU 1...\r\n");

        // Example: Send GPIO toggle command to ECU 1
        HAL_voidLedOn(RED);
        break;
    }

    case CMD_EXIT_MODE:{
        UART_SendMessage("Exiting Tester Mode...\r\n");
        break;
    }

    default:
        UART_SendMessage("Invalid Command\r\n");
        break;
    }
}

void OS_voidEnterTesterMode(void) {
    // Blink white LED twice to indicate entering Tester Mode
    OS_voidblinkWhiteLedTwice();
    CANEnable(CAN_BASE);

    UART_SendMessage("Entering Tester Mode. Send commands:\r\n");
    UART_SendMessage("1: Read DTC\r\n");
    UART_SendMessage("2: Clear DTC\r\n");
    UART_SendMessage("3: Read Sensor Data\r\n");
    UART_SendMessage("4: Test GPIO ECU2\r\n");
    UART_SendMessage("5: Test GPIO ECU1\r\n");
    UART_SendMessage("6: Exit Tester Mode\r\n");
    UART_SendMessage("Press both buttons to exit Tester Mode.\r\n");

    uint32_t command;

    while (1) {

        static uint8_t OS_ui8count = 0;
        static uint8_t currentTemp = 0;


        // Check for incoming commands
        if (UARTCharsAvail(UART0_BASE)) {
            command = UARTCharGet(UART0_BASE);  // Get the command
            processTesterCommand(command);



            // Exit on CMD_EXIT_MODE
            if (command == CMD_EXIT_MODE) {
                break;
            }
        }

        if(OS_boolReadSensorFlag)
        {
            static uint8_t keepAliveData[1] = {0xAA}; // Example data
            CAN_SendMessage(CAN_KEEP_ALIVE_ID, CAN_KEEP_ALIVE_OBJ, keepAliveData, CAN_DATA_LENGTH);

            currentTemp = OS_voidReceiveTesterMode();

            OS_ui8count++;
            if(OS_ui8count >= 100)
            {
                UART_SendMessage("Average Temperature: ");
                UART_SendNumber(currentTemp);
                UART_SendMessage("°C\r\n");
                OS_boolReadSensorFlag = false;
                OS_ui8count = 0;
            }

        }

        // Check for both buttons pressed
        if (BUTTONS_rightButton() && BUTTONS_leftButton()) {
            UART_SendMessage("Both buttons pressed. Exiting Tester Mode...\r\n");
            break;
        }

    }

    // Blink white LED twice to indicate exiting Tester Mode
    OS_voidblinkWhiteLedTwice();

    UART_SendMessage("Exited Tester Mode\r\n");
}
uint8_t OS_voidReceiveTesterMode(void)
{
    uint8_t receivedData[8];
    tCANMsgObject msgObject;
    static uint8_t temperature = 0;

    // Temporary storage for message data
    msgObject.pui8MsgData = receivedData;

    // Check Message Object 2 (Temperature data)
    CANMessageGet(CAN_BASE, CAN_TEMP_OBJ, &msgObject, true);


    if (msgObject.ui32Flags & MSG_OBJ_NEW_DATA) {

        if(msgObject.ui32MsgID == CAN_TEMP_ID)
        {
            temperature = receivedData[0];
        }
    }

    return temperature;
}
void OS_voidblinkWhiteLedTwice(void) {
    OS_ui32TesterTimer = 0;  // Reset the timer

    // Local variables to manage blinking
    uint32_t lastToggleTime = 0;
    bool ledState = false;  // Track the current LED state (on/off)

    // Blink for 2 seconds
    while (OS_ui32TesterTimer <= 2000) {
        // Check if 500ms has elapsed since the last toggle
        if ((OS_ui32TesterTimer - lastToggleTime) >= 500) {
            lastToggleTime = OS_ui32TesterTimer;  // Update the last toggle time

            // Toggle the LED state
            if (ledState) {
                HAL_voidLedOff(WHITE);  // Turn off the LED
            } else {
                HAL_voidLedOn(WHITE);   // Turn on the LED
            }
            ledState = !ledState;  // Flip the LED state
        }
    }

    // Ensure the LED is off at the end of blinking
    HAL_voidLedOff(WHITE);
}

void OS_voidCheckRXOK(void)
{
    uint32_t status = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);
    if (status & CAN_STATUS_RXOK) {
      //  HAL_voidLedOn(GREEN); // Indicate successful transmission
    }
}

void OS_voidECU1ReceiveVoltageResponse(void) {
    static uint8_t responseData[2];
    tCANMsgObject msgObject;

    msgObject.pui8MsgData = responseData;
    //UART_SendNumber(responseData[2]);
    // Check Message Object 4 for the response
    CANMessageGet(CAN_BASE,CAN_REMOTE_OBJ, &msgObject, true);

    uint32_t status = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);
    if (status & CAN_STATUS_RXOK) {
            //HAL_voidLedOn(BLUE); // Indicate successful transmission
            UART_SendNumber(responseData[0]);
        }else{}
}
/***********************************************
 * Function Name: OS_voidInit
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the operating system by calling the required
 *              initialization functions for the MCAL (Microcontroller
 *              Abstraction Layer) and HAL (Hardware Abstraction Layer).
 *              This ensures the system is set up and ready for operation.
 ***********************************************/
void OS_voidInit(void) {
    OS_voidMCALInit(); // Initialize the Microcontroller Abstraction Layer
    OS_voidHALInit();  // Initialize the Hardware Abstraction Layer
    initializeEEPROM();
}

/***********************************************
 * Function Name: OS_voidMCALInit
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the Microcontroller Abstraction Layer (MCAL)
 *              components based on the configuration flags. Each peripheral
 *              (SPI, SysTick Timer, Timer0, CAN, UART) is initialized only
 *              if its corresponding configuration flag is set to 1.
 ***********************************************/
void OS_voidMCALInit(void)
{
    #if configUSE_SPI
        SPI_masterInit();
    #endif

    #if configUSE_SYSTICK_TIMER
        SYSTICK_init();
    #endif

    #if configUSE_TIMER0
        TIMER0_init();
    #endif

    #if configUSE_CAN
        CAN_Init();
        //CAN_ReceiveInit();
        CAN_ConfigureReceiveObjects();
    #endif

    #if configUSE_UART
        UART_Init();
    #endif

//    #if configUSE_EEPROM
//        initializeEEPROM();
//    #endif
}

/***********************************************
 * Function Name: OS_voidHALInit
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the Hardware Abstraction Layer (HAL) components
 *              based on the configuration flags. Each component (LEDs and Buttons)
 *              is initialized only if its corresponding configuration flag is set to 1.
 ***********************************************/
void OS_voidHALInit(void)
{

#if configUSE_LEDS
    INITIALIZATION_leds();

#endif

#if configUSE_BUTTONS
    INITIALIZATION_buttons();
#endif

}

/***********************************************
 * Function Name: APP_voidScheduler
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: The scheduler function manages the overall state transitions
 *              and handles communication between different system components.
 *              It checks buttons, controls system states, handles CAN communication,
 *              UART control, and monitors communication status.
 ***********************************************/
void APP_voidScheduler(void)
{
    uint32_t currentTick = g_ui32SysTickCount;
    uint8_t highestPriorityIndex = MAX_TASKS;
    uint8_t i = 0;
    for ( i = 0; i < ui8taskCount; i++) {
        if (taskList[i].isEnabled && taskList[i].state == TASK_READY) {
            // Check if the task is ready to run
            if ((currentTick - taskList[i].lastRunTick) >= taskList[i].periodTicks) {

                if (highestPriorityIndex == MAX_TASKS ||
                    taskList[i].priority < taskList[highestPriorityIndex].priority) {
                    highestPriorityIndex = i;
                }
            }
        }
    }

    // Execute the highest-priority task, if any
    if (highestPriorityIndex < MAX_TASKS) {
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
void OS_voidAddTask(void (*taskFunction)(void), uint32_t periodTicks, uint8_t priority) {
    if (ui8taskCount < MAX_TASKS) {
        taskList[ui8taskCount].taskFunction = taskFunction;
        taskList[ui8taskCount].periodTicks = periodTicks;
        taskList[ui8taskCount].lastRunTick = 0;  // Initialize last run to 0
        taskList[ui8taskCount].priority = priority;
        taskList[ui8taskCount].isEnabled = 1;    // Enable by default
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


void OS_voidInitTasks(void) {

    OS_voidAddTask(OS_voidTesterMode , 200 , 1);
    OS_voidAddTask(OS_voidCheckDTC , 200 , 2);
    OS_voidAddTask(OS_voidECU1SendKeepAlive , 100 , 3);
    OS_voidAddTask(OS_voidCANHandleReceivedMessages , 200 , 4);
    OS_voidAddTask(OS_voidCheckOverheat , 200 , 5);
    OS_voidAddTask(OS_voidHeartbeatError , 200 , 6);
    OS_voidAddTask(APP_voidUartControl , 200 , 7);

    OS_voidSortTasksByPriority();
}

void OS_voidECU1SendKeepAlive(void) {

    if (g_ui32SysTickCount % 100 == 0)
    {
        uint8_t keepAliveData[1] = {0xAA}; // Example data
        CAN_SendMessage(CAN_KEEP_ALIVE_ID, CAN_KEEP_ALIVE_OBJ, keepAliveData, CAN_DATA_LENGTH);
        //CAN_SendMessage(CAN_KEEP_ALIVE_ID, CAN_KEEP_ALIVE_OBJ, keepAliveData, CAN_DATA_LENGTH);
    }

    if(OS_ui32CommLostTimer >= 5000)
    {
        OS_boolBlinkBlueFlag = true;
    }

}
void OS_voidHeartbeatError(void)
{
    if(OS_boolBlinkBlueFlag && !OS_boolCommunicationDTCFlag && !OS_boolVoltageDTCFlag && !OS_boolDTCFlag)
    {

        if(!OS_boolIncrementCommFlag){
            OS_boolIncrementCommFlag = true;
            NVM_voidIncrementCommunicationDTCCounter();
        }

        if(OS_ui32BlinkBlueTimer >= 20000)
        {
            HAL_voidLedOff(BLUE);
            OS_boolBlinkBlueFlag = false;
            OS_ui32CommLostTimer = 0;
        }
        UartState = COMMUNICATION_LOST_STATE;
        HAL_voidLedBlink(BLUE);
    }
    else
    {
        HAL_voidLedOff(BLUE);
        OS_ui32BlinkBlueTimer = 0;

    }


    if(OS_ui32CommFailure >= 30000)
    {
        UartState = COMMUNICATION_FAILURE;
    }
}

void OS_voidDebugMessageObjects(void) {
    tCANMsgObject msgObject;
    uint8_t dummyData[8];
    msgObject.pui8MsgData = dummyData;
    uint8_t objID;

    for (objID = 1; objID <= 4; objID++) { // Adjust based on your objects
        CANMessageGet(CAN_BASE, objID, &msgObject, true);

        UART_SendMessage("Object Number: ");
        UART_SendNumber(objID);
        UART_SendMessage(", Message ID: ");
        UART_SendNumber(msgObject.ui32MsgID);
        UART_SendMessage(", Mask: ");
        UART_SendNumber(msgObject.ui32MsgIDMask);
        UART_SendMessage("\n");
    }
}

/***********************************************
 * Function Name: APP_voidSendToPc
 * Inputs: uint32_t a_uint32data (State or communication code)
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function sends a message to the PC via UART depending on the
 *              state passed as input (`a_uint32data`).
 ***********************************************/
void APP_voidSendToPc(uint32_t a_uint32data)
{
    // Switch based on the state or communication code passed
    switch (a_uint32data)
    {
        case NORMAL_STATE:
            UART_send("Normal State\r\n");
            break;

        case FAULT_STATE:
            UART_send("Fault State\r\n");
            break;

        case SENSOR_DAMAGED:
            UART_send("SENSOR DAMAGED\r\n");
            break;

        case COMMUNICATION_LOST_STATE:
            UART_send("COMMUNICATION LOST !!!!\r\n");
            break;

        case OVERHEAT:
            UART_send("OVERHEAT SHUT DOWN MOTOR!!!\r\n");
            break;

        case COMMUNICATION_FAILURE:
            UART_send("30 seconds passed communication failure\r\n");
            break;


        default:
            // Handle unexpected states (optional: log or notify)
//            UART_send("Unknown State Received\r\n");
            break;
    }
}

/***********************************************
 * Function Name: OS_voidCanSend
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function sends the current value of `APP_ui32counter`
 *              over CAN if the state is `UNIDENTIFIED`. It also tracks the
 *              number of messages sent and resets counters after 50 messages.
 ***********************************************/
void OS_voidCanSend(void)
{
    // Always send the current counter value over CAN, regardless of the state
    //CAN_Send(CAN_TX_MESSAGE_ID, &APP_ui32counter, CAN_DATA_LENGTH);
}

/***********************************************
 * Function Name: OS_voidCANReceiveMessage
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function receives a CAN message and processes it based on
 *              the error state in the received data. It sets flags for specific
 *              error conditions, like `UNIDENTIFIED` or `DONT_TOUCH`.
 ***********************************************/
void OS_voidCANReceiveMessage(void) {
    tCANMsgObject messageObject;
    // Prepare to receive the message
    messageObject.pui8MsgData = g_ui8ReceivedData;

    // Get the new CAN message
    CANMessageGet(CAN_BASE, CAN_RX_OBJECT_NUM, &messageObject, true);

    // Check for the first byte in the received data
    OS_ui32CurrentError = g_ui8ReceivedData[0];

}

/***********************************************
 * Function Name: APP_voidUartControl
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function controls the state transitions for UART communication.
 *              It checks various flags, updates the `UartState`, and sends the
 *              corresponding state messages to the PC.
 ***********************************************/
void APP_voidUartControl(void)
{
//    // Check for state change and send corresponding messages
//    if (UartState != APP_ui32UartCurrentState)
//    {
//
//
//        // Update the current UART state
//        APP_ui32UartCurrentState = UartState;
//    }
    if(g_ui32SysTickCount % 1000 == 0)
    {
        APP_voidSendToPc(UartState);
    }else{

    }



}

/***********************************************
 * Function Name: OS_voidCheckCANCommunication
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Checks the CAN communication status.
 *              Updates the `commcheck` flag based on acknowledgment
 *              or transmission success. Logs unexpected states if encountered.
 ***********************************************/
void OS_voidCheckCANCommunication(void)
{
    uint32_t ui32CANStatus;

    // Get the current status of the CAN controller
    ui32CANStatus = CANStatusGet(CAN_BASE, CAN_STS_CONTROL);

    // Check for an acknowledgment error
    if (ui32CANStatus & CAN_STATUS_LEC_ACK) {
        commcheck = false;
    }
    // Check for successful transmission
    else if (ui32CANStatus & CAN_STATUS_TXOK) {
        commcheck = true;
    }
    // Handle other unexpected states
    else {
        // Optional: Log the unexpected status for debugging
//        UART_send("Unexpected CAN status encountered!\r\n");
        // You can also handle other statuses here if needed
    }
}

/***********************************************
 * Function Name: APP_voidCheckCommunication
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Checks the communication status and updates the current state.
 *              Handles transition to the `COMM_LOST` state when communication fails.
 *              Logs the communication loss for debugging.
 ***********************************************/
void APP_voidCheckCommunication(void)
{
    if (commcheck)
    {
        if (APP_ui32CurrentState == COMM_LOST)
        {
            APP_ui32counter = APP_ui32PrevState;
        }
        APP_ui32CurrentState = APP_ui32PrevState;
    }
    else if (!commcheck)
    {
        APP_ui32counter = COMM_LOST;
        UartState = COMM_LOST;
        APP_ui32CurrentState = COMM_LOST;

        // Log communication loss
        // UART_send("Communication Lost!\r\n");
    }
    else
    {
        // Optional: Handle unexpected states if needed
    }

    // If communication is lost, trigger a red LED blink
    if (APP_ui32counter == COMM_LOST) {
        HAL_voidLedBlink(RED);
    }else{}

}

/***********************************************
 * Function Name: initButtons
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
 * Function Name: UART_send
 * Inputs: const char *array_ptr - Pointer to the character array (string) to be sent
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Sends a string through UART0 by calling the UART0_sendMessage function.
 ***********************************************/
void UART_send(const char *array_ptr)
{
    UART_SendMessage(array_ptr);
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


