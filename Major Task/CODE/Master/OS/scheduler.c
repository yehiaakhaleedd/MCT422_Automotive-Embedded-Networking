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
uint32_t APP_ui32holdButton1count = 0;
uint32_t APP_ui32holdButton2count = 0;
uint32_t OS_ui32CurrentError = 0;
uint32_t OS_ui32PrevError = 0x12;
uint32_t OS_ui32index = 0;
uint32_t OS_ui32time = 100;


static uint8_t APP_ui8PrevState = 0;

bool APP_boollast_sw1_state = false;
bool APP_boollast_sw2_state = false;
bool OS_boolidleFlag = false;
bool APP_boolStateInit = false;
bool APP_boolUnidentifiedFlag = false;
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

    // Increment the idle counter (can be used to track system inactivity)
    APP_ui32idleCounter++;

    // Increment the counters for tracking the duration of button presses
    APP_ui32holdButton1count++;
    APP_ui32holdButton2count++;
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
    // Static variable to track if initialization has been performed
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
    if (g_ui32SysTickCount % (uint32_t)50 == 0x00U) {
        //APP_voidScheduler(); // Call the scheduler function
        OS_voidSchedulerr();
    } else {
        // No action required if not a multiple of 50 ms
    }
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
    CAN_ReceiveInit();
    #endif

    #if configUSE_UART
    UART_Init();
    #endif
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
    OS_voidSchedulerr();
}

/***********************************************
 * Function Name: OS_voidAddTask
 * Inputs:
 *  - taskFunction: Pointer to the task function to be added
 *  - periodTicks: Periodicity of the task in ticks
 *  - priority: Priority of the task (lower value indicates higher priority)
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Adds a new task to the task list with the specified function, periodicity,
 *              and priority. Initializes task parameters like last run tick, state,
 *              and enables the task by default.
 ***********************************************/

void OS_voidAddTask(void (*taskFunction)(void), uint32_t periodTicks, uint8_t priority) {
    // Check if there is space for more tasks in the task list
    if (ui8taskCount < MAX_TASKS) {
        // Assign the task function to the corresponding slot in the task list
        taskList[ui8taskCount].taskFunction = taskFunction;

        // Set the task's periodicity (in ticks)
        taskList[ui8taskCount].periodTicks = periodTicks;

        // Initialize the last run tick to 0
        taskList[ui8taskCount].lastRunTick = 0x00U;

        // Set the task's priority
        taskList[ui8taskCount].priority = priority;

        // Enable the task by default
        taskList[ui8taskCount].isEnabled = 0x01U;

        // Set the task's initial state to READY
        taskList[ui8taskCount].state = TASK_READY;

        // Increment the task count to reflect the addition of a new task
        ui8taskCount++;
    }
}

/***********************************************
 * Function Name: OS_voidSortTasksByPriority
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synchronous
 * Description: Sorts the tasks in the task list based on their priority in ascending order.
 *              Lower priority values are considered higher priority tasks.
 ***********************************************/

void OS_voidSortTasksByPriority(void) {
    uint8_t i = 0;
    uint8_t j = 0;

    // Loop through each task in the task list
    for (i = 0; i < ui8taskCount - 1; i++) {
        // Compare with each subsequent task to find the lowest priority task
        for (j = i + 1; j < ui8taskCount; j++) {
            // If the current task's priority is higher than the next task's priority, swap them
            if (taskList[i].priority > taskList[j].priority) {
                // Temporary variable to store the task being swapped
                TaskControlBlock temp = taskList[i];

                // Swap the two tasks in the list
                taskList[i] = taskList[j];
                taskList[j] = temp;
            }
        }
    }
}

/***********************************************
 * Function Name: OS_voidSchedulerr
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synchronous
 * Description: This function is responsible for selecting and executing the highest-priority
 *              task from the task list based on the current system tick and task readiness.
 ***********************************************/

void OS_voidSchedulerr(void) {
    uint32_t currentTick = g_ui32SysTickCount;  // Get the current system tick count
    uint8_t highestPriorityIndex = MAX_TASKS;   // Initialize index for highest priority task
    uint8_t i = 0;  // Loop variable

    // Conditions to check if the task is enabled, ready, and if priority conditions are met
    bool check_en = (taskList[i].isEnabled);
    bool check_task = (taskList[i].state == TASK_READY);
    bool check_priority = (highestPriorityIndex == MAX_TASKS);
    bool list = (taskList[i].priority < taskList[highestPriorityIndex].priority);

    // Loop through all tasks to find the task with the highest priority
    for (i = 0; i < ui8taskCount; i++) {
        // Check if the task is enabled and ready to run
        if (check_en && check_task) {
            // Check if enough time has passed to run the task based on its period
            if ((currentTick - taskList[i].lastRunTick) >= taskList[i].periodTicks) {
                // If it's the first task or the current task has a higher priority, update the highestPriorityIndex
                if (check_priority || list) {
                    highestPriorityIndex = i;
                }
            }
        }
    }

    // Execute the highest-priority task if one is found
    if (highestPriorityIndex < MAX_TASKS) {
        // Mark the task as running
        taskList[highestPriorityIndex].state = TASK_RUNNING;

        // Execute the task's function
        taskList[highestPriorityIndex].taskFunction();

        // Update the last run tick for the task
        taskList[highestPriorityIndex].lastRunTick = currentTick;

        // Set the task's state back to READY after execution
        taskList[highestPriorityIndex].state = TASK_READY;
    } else {
        // If no tasks are ready, the idle task can be run (not implemented here)
        // OS_IdleTask();
    }

    // Optionally, if no tasks are ready, the idle task can be run
    // OS_IdleTask();
}

/***********************************************
 * Function Name: OS_voidInitTasks
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synchronous
 * Description: This function initializes the tasks by adding them to the task list
 *              with their respective functions, periods, and priorities.
 *              It then sorts the tasks based on their priorities.
 ***********************************************/

void OS_voidInitTasks(void) {
    // Add tasks to the task list with the respective function, period, and priority
    OS_voidAddTask(&OS_voidCanSend, OS_ui32time, (uint8_t)1);           // Task to send CAN messages
    OS_voidAddTask(&APP_ui32CheckButtons, OS_ui32time, (uint8_t)2);      // Task to check button states
    OS_voidAddTask(&APP_voidStateControl, OS_ui32time, (uint8_t)3);      // Task to control the state
    OS_voidAddTask(&OS_voidCANReceiveMessage, OS_ui32time, (uint8_t)4);  // Task to receive CAN messages
    OS_voidAddTask(&APP_voidUartControl, OS_ui32time, (uint8_t)5);       // Task to control UART
    OS_voidAddTask(&OS_voidCheckCANCommunication, OS_ui32time, (uint8_t)6);  // Task to check CAN communication status
    OS_voidAddTask(&APP_voidCheckCommunication, OS_ui32time, (uint8_t)7);  // Task to check overall communication
    OS_voidAddTask(&OS_voidCheckIdle, OS_ui32time, (uint8_t)8);          // Task to check idle state

    // Sort the tasks based on their priorities
    OS_voidSortTasksByPriority();
}


/***********************************************
 * Function Name: APP_voidStateControl
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synchronous
 * Description: This function calls `APP_voidStateControll` with the current value of `APP_ui32counter` to control the state.
 ***********************************************/

void APP_voidStateControl(void)
{
    // Call the state control function with the current value of APP_ui32counter
    APP_voidStateControll((uint32_t)APP_ui32counter);
}

/***********************************************
 * Function Name: APP_ui32CheckButtons
 * Inputs: N/A
 * Outputs: uint32_t state (return value, not used in the function logic)
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function checks the status of two buttons (sw1 and sw2),
 *              handles button presses, and updates the system counter.
 *              It also manages flags for button hold events and ensures the system
 *              transitions to the appropriate states based on button inputs.
 ***********************************************/
void APP_ui32CheckButtons(void)
{
    // Read the current button states
    sw1 = (bool)BUTTONS_rightButton();
    sw2 = (bool)BUTTONS_leftButton();

    bool is_sw1_pressed = (sw1 == 0x01U);
    bool is_sw2_pressed = (sw1 == 0x01U);
    bool is_long_press = (APP_ui32holdButton1count >= (uint32_t)3000);
    // Handle long button hold events (buttons held for more than 3000ms)
    if (is_sw1_pressed  && is_long_press) {
        holdButtonFlag = true;
        LED_greenOn();
    }

    if (is_sw2_pressed && is_long_press) {
        holdButtonFlag = true;
        LED_greenOn();
    }

    bool sw1_on = (sw1 == true);
    bool sw2_on = (sw2 == true);
    bool last_sw1_state_off = (APP_boollast_sw1_state == false);
    bool last_sw2_state_off = (APP_boollast_sw2_state == false);


    // Handle button press detection for both sw1 and sw2
    if (sw1_on && last_sw1_state_off && sw2_on && last_sw2_state_off) {
        // If both buttons are pressed simultaneously, reset the counter to UNIDENTIFIED
        APP_ui32counter = UNIDENTIFIED;
        APP_ui32idleCounter = 0x00U;
    }
    else if (sw1_on && last_sw1_state_off) {
        // If sw1 is pressed, increment the counter and reset idle counter
        APP_ui32idleCounter = 0x00U;
        APP_ui32counter++;

        // If the counter exceeds 3, reset it to STATE_RED
        if (APP_ui32counter > 0x03U) {
            APP_ui32counter = STATE_RED;
        }
    }
    else if (sw2_on && last_sw2_state_off) {
        // If sw2 is pressed, decrement the counter and reset idle counter
        APP_ui32idleCounter = 0x00U;
        APP_ui32counter--;

        // If the counter goes below 1, set it to STATE_BLUE
        if (APP_ui32counter < 0x01U) {
            APP_ui32counter = STATE_BLUE;
        }
    }
    else{}

    // Update the last state of the buttons for next iteration
    APP_boollast_sw1_state = sw1;
    APP_boollast_sw2_state = sw2;
}

/***********************************************
 * Function Name: APP_voidStateControl
 * Inputs: uint32_t a_ui32counter (The current state counter)
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function checks the input state counter (`a_ui32counter`)
 *              and updates the state of the system. It also sends the state
 *              to a PC in case of an initialization state.
 ***********************************************/
void APP_voidStateControll(uint32_t a_ui32counter)
{
    // Switch on the input state counter to control system behavior
    switch (a_ui32counter)
    {
        case STATE_RED:
            // Update UART and previous state to reflect the current red state
            UartState = STATE_RED;
            APP_ui8PrevState = STATE_RED;
            break;

        case STATE_GREEN:
            // Update UART and previous state to reflect the current green state
            UartState = STATE_GREEN;
            APP_ui8PrevState = STATE_GREEN;
            break;

        case STATE_BLUE:
            // Update UART and previous state to reflect the current blue state
            UartState = STATE_BLUE;
            APP_ui8PrevState = STATE_BLUE;
            break;

        case STATE_INIT:
            // Send the initialization state to the PC
            APP_voidSendToPc(a_ui32counter);
            // Reset the state to STATE_RED after initialization
            APP_ui32counter = STATE_RED;
            break;


        default:
            // Handle unexpected states, if necessary (optional)
            // You can log an error or set to a default state
            break;
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
        case STATE_RED:
            UART_send("State Machine Is Red\r\n");
            break;

        case STATE_GREEN:
            UART_send("State Machine Is Green\r\n");
            break;

        case STATE_BLUE:
            UART_send("State Machine Is Blue\r\n");
            break;

        case COMM_LOST:
            UART_send("COMMUNICATION LOST!!!!\r\n");
            break;

        case UNIDENTIFIED:
            UART_send("UNIDENTIFIED STATE!!!\r\n");
            break;

        case DONT_TOUCH:
            UART_send("DON'T TOUCH SLAVE BUTTONS!!!\r\n");
            break;

        case STATE_WHITE:
            UART_send("Slave Is In Idle State\r\n");
            break;

        case STATE_INIT:
            UART_send("State Machine Is In Initial State\r\n");
            break;

        case RECEIVED_COMM:
            UART_send("Communication Received\r\n");
            break;

        case LEAVE_BUTTON:
            UART_send("LEAVE MASTER BUTTON!!\r\n");
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
    // Check if the state is UNIDENTIFIED
    if (APP_ui32counter == UNIDENTIFIED)
    {
        // Send the current counter value over CAN
        CAN_Send(CAN_TX_MESSAGE_ID, &APP_ui32counter, CAN_DATA_LENGTH);

        // Increment the message send index
        OS_ui32index++;

        // Reset the index and counter after sending 50 messages
        if (OS_ui32index >= 0x10U)
        {
            OS_ui32index = 0x00U;        // Reset the index
            APP_ui32counter = 0x00U;      // Reset the counter
            OS_ui32PrevError = 0x00U;     // Reset the error state
        }
    }

    // Always send the current counter value over CAN, regardless of the state
    CAN_Send(CAN_TX_MESSAGE_ID, &APP_ui32counter, CAN_DATA_LENGTH);
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
    static uint8_t g_ui8ReceivedData[CAN_DATA_LENGTH] = {0};

    tCANMsgObject messageObject;

    // Prepare to receive the message
    messageObject.pui8MsgData = g_ui8ReceivedData;

    // Get the new CAN message
    CANMessageGet(CAN_BASE, (uint32_t)CAN_RX_OBJECT_NUM, &messageObject, true);

    // Check for the first byte in the received data
    OS_ui32CurrentError = g_ui8ReceivedData[0];


    bool check_undef = (g_ui8ReceivedData[0] == UNIDENTIFIED);
    bool check_state = (OS_ui32CurrentError != OS_ui32PrevError);
    bool check_slaveButtons = (g_ui8ReceivedData[0] == DONT_TOUCH);
    // Check if the received data is UNIDENTIFIED and if the error state has changed
    if (check_undef && check_state) {
        APP_boolUnidentifiedFlag = true; // Set the unidentified flag
        OS_ui32PrevError = UNIDENTIFIED; // Update the previous error
    }
    // Check if the received data is DONT_TOUCH and if the error state has changed
    else if (check_slaveButtons && check_state) {
        SlaveButtonsFlag = true; // Set the slave button flag
        OS_ui32PrevError = DONT_TOUCH; // Update the previous error
    }
    else {
        // Reset flags if no error condition is met
        APP_boolUnidentifiedFlag = false;
        SlaveButtonsFlag = false;
    }
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
    bool state_check = (UartState == COMM_LOST);
    bool comm = (commcheck);
    // Handle communication lost state
    if (state_check && comm)
    {
        HAL_voidLedOff(RED);
        recievedFlag = true;
        APP_ui32counter = 0x00U;
    }

    // Handle button hold event
    if (holdButtonFlag)
    {
        UartState = LEAVE_BUTTON;
        APP_ui8PrevState = LEAVE_BUTTON;
        APP_ui32counter = 0x00U;
    }

    // Handle white flag event
    if (whiteFlag)
    {
        UartState = STATE_WHITE;
        APP_ui8PrevState = STATE_WHITE;
    }

    // Handle slave button press event
    if (SlaveButtonsFlag)
    {
        UartState = DONT_TOUCH;
        APP_ui8PrevState = DONT_TOUCH;
        APP_ui32counter = 0x00U;
    }

    // Handle state initialization event
    if (APP_boolStateInit)
    {
        UartState = STATE_INIT;
        APP_ui8PrevState = STATE_INIT;
    }

    bool undef = (APP_boolUnidentifiedFlag);
    bool recieved = (!recievedFlag);
    bool comm_received = (UartState != RECEIVED_COMM);
    // Handle unidentified state and communication received event
    if (undef && recieved && comm_received)
    {
        UartState = UNIDENTIFIED;
        APP_ui8PrevState = UNIDENTIFIED;
    }

    // Check for state change and send corresponding messages
    if (UartState != APP_ui32UartCurrentState)
    {
        // Send message for unidentified state
        if (undef && recieved && comm_received)
        {
            UartState = UNIDENTIFIED;
            APP_ui8PrevState = UNIDENTIFIED;
            APP_voidSendToPc(UNIDENTIFIED);
            unidentifiedFlag = false;
        }

        // Send message for leave button state
        if (holdButtonFlag)
        {
            UartState = LEAVE_BUTTON;
            APP_ui8PrevState = LEAVE_BUTTON;
            APP_voidSendToPc(LEAVE_BUTTON);
        }

        // Send message for slave button press state
        if (SlaveButtonsFlag)
        {
            UartState = DONT_TOUCH;
            APP_ui8PrevState = DONT_TOUCH;
            APP_voidSendToPc(DONT_TOUCH);
        }

        // Send message for white flag state
        if (whiteFlag)
        {
            UartState = STATE_WHITE;
            APP_ui8PrevState = STATE_WHITE;
            APP_voidSendToPc(STATE_WHITE);
            whiteFlag = false;
        }

        // Send message for received communication state
        if (recievedFlag)
        {
            UartState = RECEIVED_COMM;
            APP_ui8PrevState = RECEIVED_COMM;
            APP_voidSendToPc(RECEIVED_COMM);
            recievedFlag = false;
        }

        // Send message for state initialization state
        if (APP_boolStateInit)
        {
            UartState = STATE_INIT;
            APP_ui8PrevState = STATE_INIT;
            APP_voidSendToPc(STATE_INIT);
            APP_boolStateInit = false;
        }

        bool def = (!APP_boolUnidentifiedFlag);
        bool slave_buttons = (!SlaveButtonsFlag);
        bool idle = (!whiteFlag);
        bool unheld_buttons = (!holdButtonFlag);
        bool state_init = (!APP_boolStateInit);
        // Send counter value if no flags are set
        if (def && slave_buttons && idle && recieved && unheld_buttons && state_init)
        {
            APP_voidSendToPc((uint32_t)APP_ui32counter);
        }

        // Update the current UART state
        APP_ui32UartCurrentState = UartState;
    }

    bool is_sw1_unpressed = (sw1 == 0x00U);
    bool is_sw2_unpressed = (sw2 == 0x00U);

    // Reset button flags when buttons are released
    if (is_sw1_unpressed)
    {
        APP_ui32holdButton1count = 0x00U;
        holdButtonFlag = false;
        LED_greenOff();
    }

    if (is_sw2_unpressed)
    {
        APP_ui32holdButton2count = 0x00U;
        holdButtonFlag = false;
        LED_greenOff();
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
    static uint32_t APP_ui32CurrentState =0; //CHECK PLACE
    if (commcheck)
    {
        if (APP_ui32CurrentState == COMM_LOST)
        {
            APP_ui32counter = APP_ui8PrevState;
        }
        APP_ui32CurrentState = APP_ui8PrevState;
    }
    else if (!commcheck)
    {
        APP_ui32counter = COMM_LOST;
        UartState = COMM_LOST;
        APP_ui32CurrentState = COMM_LOST;

        // Log communication loss
//        UART_send("Communication Lost!\r\n");
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
 * Function Name: OS_voidCheckIdle
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Checks if the system is in an idle state.
 *              If the idle counter reaches a specific threshold,
 *              sets the system to `STATE_WHITE` and sends a CAN message.
 ***********************************************/
void OS_voidCheckIdle(void)
{
    if (APP_ui32idleCounter >= (uint32_t)10000)
    {
        OS_boolidleFlag = true;
        APP_ui32idleCounter = 0x00U;
    }
    else {}

    if (OS_boolidleFlag)
    {
        whiteFlag = true;
        OS_boolidleFlag = false;
        APP_ui32counter = STATE_WHITE;
        CAN_Send(CAN_TX_MESSAGE_ID, &APP_ui32counter, CAN_DATA_LENGTH);
    }
    else {}
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




