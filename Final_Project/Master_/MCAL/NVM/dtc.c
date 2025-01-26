//#include <stdint.h>
//#include <stdbool.h>
//#include "MCAL/NVM/NVM.h"
//#include "HAL/led.h"
//#include "HAL/buttons.h"
//#include "MCAL/ADC/ADC.h"
//#include "MCAL/UART/uart.h"
//
////bool sw1; // Right button
////bool sw2; // Left button
////bool APP_boollast_sw1_state = false;
////bool APP_boollast_sw2_state = false;
//
//
//// Function prototypes
//void systemInit(void);
//void mainLoop(void);
//void processTesterCommand(uint32_t command);
//void enterTesterMode(void);
//
////int main(void) {
////    // Initialize the system
////    systemInit();
////
////    // Enter the main application loop
////    mainLoop();
////
////    return 0;
////}
//
//// System Initialization
//void systemInit(void) {
//    // Initialize peripherals
//    LED_initPortF();         // Initialize LEDs
//    BUTTONS_initPortF();     // Initialize buttons
//    initializeEEPROM();      // Initialize EEPROM and read counter
//    UART_Init();
//
//    // Turn off all LEDs initially
//    HAL_voidLedOff(RED);
//    HAL_voidLedOff(GREEN);
//    HAL_voidLedOff(BLUE);
//
//    UART_SendMessage("Start");
//
//}
//
//// Main application loop
//void mainLoop(void) {
//    while (1) {
//        APP_ui32CheckButtons();
//
//        // Small delay to avoid rapid execution
//        SysCtlDelay(SysCtlClockGet() / 1000);
//    }
//}
//
//void processTesterCommand(uint32_t command) {
//
//    switch (command) {
//    case CMD_READ_DTC:
//        UART_SendMessage("Reading DTC...\r\n");
//        readDTC();
//        break;
//
//    case CMD_CLEAR_DTC:
//        UART_SendMessage("Clearing DTC...\r\n");
//        clearDTC();
//        break;
//
//    case CMD_READ_SENSOR: {
//        UART_SendMessage("Reading Sensor Data...\r\n");
//
//        // Get temperature value as an integer
//        int32_t sensor_value = (int32_t)getTemperature();  // Map and cast the sensor value to an integer
//
//        // Send the integer sensor value via UART
//        UART_SendMessage("Temperature: ");
//        UART_SendNumber(sensor_value);  // Send the integer part only
//        UART_SendMessage("°C\r\n");
//        break;
//    }
//
//    case CMD_TEST_GPIO:
//        UART_SendMessage("Testing GPIO...\r\n");
//        HAL_voidLedOn(GREEN);
//        break;
//
//    case CMD_EXIT_MODE:
//        UART_SendMessage("Exiting Tester Mode...\r\n");
//        break;
//
//    default:
//        UART_SendMessage("Invalid Command\r\n");
//        break;
//    }
//}
//
//void enterTesterMode(void) {
//    UART_SendMessage("Entering Tester Mode. Send commands:\r\n");
//    UART_SendMessage("1: Read DTC\r\n");
//    UART_SendMessage("2: Clear DTC\r\n");
//    UART_SendMessage("3: Read Sensor Data\r\n");
//    UART_SendMessage("4: Test GPIO\r\n");
//    UART_SendMessage("FF: Exit Tester Mode\r\n");
//
//    uint8_t command;
//
//    while (1) {
//        // Check for incoming commands
//        if (UARTCharsAvail(UART0_BASE)) {
//            command = UARTCharGet(UART0_BASE);  // Get the command
//            processTesterCommand(command);
//
//            // Exit on CMD_EXIT_MODE
//            if (command == CMD_EXIT_MODE) {
//                break;
//            }
//        }
//    }
//
//    UART_SendMessage("Exited Tester Mode\r\n");
//}
//
//
//void APP_ui32CheckButtons(void)
//{
//    // Read the current button states
//    sw1 = (bool)BUTTONS_rightButton();
//    sw2 = (bool)BUTTONS_leftButton();
//
//     if (sw1 == true && APP_boollast_sw1_state == false) {
//        // If sw1 is pressed, increment the counter and reset idle counter
//        UART_SendMessage("Entering Tester Mode...\r\n");
//        enterTesterMode();  // Enter Tester Mode
//
//        }
//    else if (sw2 == true && APP_boollast_sw2_state == false) {
//        // If sw2 is pressed, decrement the counter and reset idle counter
//        UART_SendMessage("Incrementing Counter...\r\n");
//       // incrementButtonPressCounter();  // Increment counter on right button press
//        }
//    // Update the last state of the buttons for next iteration
//    APP_boollast_sw1_state = sw1;
//    APP_boollast_sw2_state = sw2;
//
//}
