#include "NVM.h"

static uint32_t OS_ui8OverheatDTCCounter = 0;
static uint32_t OS_ui8CommunicationDTCCounter = 0;
static uint32_t g_DTC = 0;



volatile bool g_Fault_Flag = false;
bool OS_boolDTCFlag = false;
bool OS_boolVoltageDTCFlag = false;
bool OS_boolCommunicationDTCFlag = false;
void resetButtonPressCounter(void) {
    OS_ui8OverheatDTCCounter = 0;
    g_DTC = 0;

    // Reset the button press counter and DTC in EEPROM
    EEPROMProgram(&OS_ui8OverheatDTCCounter, EEPROM_BUTTON_COUNTER_ADDR, sizeof(OS_ui8OverheatDTCCounter));
    EEPROMProgram(&g_DTC, EEPROM_DTC_ADDR, sizeof(g_DTC));

    // Turn off the red LED and reset the fault flag
    HAL_voidLedOff(RED);
    g_Fault_Flag = false;
}

void checkButtonPresses(void) {
    bool sw1 = BUTTONS_rightButton(); // Right button
    static bool lastButtonState = false;  // Previous button state

    // Detect button press transition
    if (sw1 && !lastButtonState) {
        NVM_voidIncrementDTCCounter();  // Increment counter when button is pressed
    }

    // Update the last state of the button
    lastButtonState = sw1;
}

void failSafeState(void) {
    while (g_Fault_Flag) {
        HAL_voidLedOn(RED);  // Turn on the red LED
        checkButtonPresses();  // Allow button press detection for resetting
    }
}

void NVM_voidIncrementDTCCounter(void) {
    // Read the button press counter from EEPROM
    EEPROMRead(&OS_ui8OverheatDTCCounter, EEPROM_BUTTON_COUNTER_ADDR, sizeof(OS_ui8OverheatDTCCounter));

    // Increment the counter
    OS_ui8OverheatDTCCounter++;

    // Send the value over UART
    UART_SendMessage("Current overheat counter: ");
    UART_SendNumber(OS_ui8OverheatDTCCounter);
    UART_SendMessage("\r\n");


    // Check if the counter has reached 3
    if (OS_ui8OverheatDTCCounter >= 3) {
        g_DTC = 1;  // Set the DTC value
        EEPROMProgram(&g_DTC, EEPROM_DTC_ADDR, sizeof(g_DTC));  // Save DTC to EEPROM

//        // Blink the red LED for 3 seconds to indicate the fault
//        HAL_voidLedOn(RED);
//        g_Fault_Flag = true;  // Set fault flag
//        SysCtlDelay(SysCtlClockGet() * 3);
//        HAL_voidLedOff(RED);
        // Send the value over UART

        if(g_DTC == 1)
        {
            UART_send("I am in NVM\r\n");
            OS_boolDTCFlag = true;
        }


        UART_SendMessage("Current DTC: ");
        UART_SendNumber(g_DTC);
        UART_SendMessage("\r\n");


        // Reset the button press counter
        OS_ui8OverheatDTCCounter = 0;
    }

    // Save the updated counter to EEPROM
    EEPROMProgram(&OS_ui8OverheatDTCCounter, EEPROM_BUTTON_COUNTER_ADDR, sizeof(OS_ui8OverheatDTCCounter));
}

void NVM_voidIncrementCommunicationDTCCounter(void) {
    // Read the button press counter from EEPROM
    EEPROMRead(&OS_ui8CommunicationDTCCounter, EEPROM_COMM_COUNTER_ADDR, sizeof(OS_ui8CommunicationDTCCounter));

    // Increment the counter
    OS_ui8CommunicationDTCCounter++;

    // Send the value over UART
    UART_SendMessage("Current communication counter: ");
    UART_SendNumber(OS_ui8CommunicationDTCCounter);
    UART_SendMessage("\r\n");


    // Check if the counter has reached 3
    if (OS_ui8CommunicationDTCCounter >= 3) {
        g_DTC = 3;  // Set the DTC value
        EEPROMProgram(&g_DTC, EEPROM_DTC_ADDR, sizeof(g_DTC));  // Save DTC to EEPROM

//        // Blink the red LED for 3 seconds to indicate the fault
//        HAL_voidLedOn(RED);
//        g_Fault_Flag = true;  // Set fault flag
//        SysCtlDelay(SysCtlClockGet() * 3);
//        HAL_voidLedOff(RED);
        // Send the value over UART

        if(g_DTC == 3)
        {
            UART_send("I am in communication NVM\r\n");
            OS_boolCommunicationDTCFlag = true;
        }


        UART_SendMessage("Current DTC: ");
        UART_SendNumber(g_DTC);
        UART_SendMessage("\r\n");


        // Reset the button press counter
        OS_ui8CommunicationDTCCounter = 0;
    }

    // Save the updated counter to EEPROM
    EEPROMProgram(&OS_ui8CommunicationDTCCounter, EEPROM_COMM_COUNTER_ADDR, sizeof(OS_ui8CommunicationDTCCounter));
}

void checkFaultFlag(void) {
    if (g_Fault_Flag) {
        // Indicate fault with blue LED for 2 seconds
        HAL_voidLedOn(BLUE);
        SysCtlDelay(SysCtlClockGet() * 2);
        HAL_voidLedOff(BLUE);

        // Clear the fault flag after handling
        g_Fault_Flag = false;
    }
}

void initializeEEPROM(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)) {}

    EEPROMInit();  // Ensure EEPROM is ready for access

    // Read DTC value from EEPROM
    EEPROMRead(&g_DTC, EEPROM_DTC_ADDR, sizeof(g_DTC));

    // Initialize DTC state
    if (g_DTC == 1) {
        OS_boolDTCFlag = true;
    }
    else if(g_DTC == 2)
    {
        OS_boolVoltageDTCFlag = true;
    }else if(g_DTC == 3)
    {
        OS_boolCommunicationDTCFlag = true;
    }
    // Read the current button press counter from EEPROM
    EEPROMRead(&OS_ui8OverheatDTCCounter, EEPROM_BUTTON_COUNTER_ADDR, sizeof(OS_ui8OverheatDTCCounter));
    // If the counter is uninitialized, set it to 0
    if (OS_ui8OverheatDTCCounter == 0xFFFFFFFF) {
        OS_ui8OverheatDTCCounter = 0;
        EEPROMProgram(&OS_ui8OverheatDTCCounter, EEPROM_BUTTON_COUNTER_ADDR, sizeof(OS_ui8OverheatDTCCounter));
    }
}

uint32_t readDTC(void) {
    uint32_t dtc_value = 0;

    // Read the DTC value from EEPROM
    EEPROMRead(&dtc_value, EEPROM_DTC_ADDR, sizeof(dtc_value));

    // Send the value over UART
    UART_SendMessage("Current DTC: ");
    UART_SendNumber(dtc_value);
    UART_SendMessage("\r\n");

    return dtc_value;
}

void clearDTC(void) {
    g_DTC = 0;  // Reset the DTC in RAM
    EEPROMProgram(&g_DTC, EEPROM_DTC_ADDR, sizeof(g_DTC));  // Reset in EEPROM
    UART_SendMessage("DTC Cleared Successfully\r\n");

    OS_ui8CommunicationDTCCounter = 0;
    EEPROMProgram(&OS_ui8CommunicationDTCCounter, EEPROM_COMM_COUNTER_ADDR, sizeof(OS_ui8CommunicationDTCCounter));


}

