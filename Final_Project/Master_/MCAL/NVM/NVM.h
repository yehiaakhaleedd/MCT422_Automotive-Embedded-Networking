#ifndef DTC_MANAGER_H
#define DTC_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/eeprom.h"
#include "driverlib/sysctl.h"
#include "HAL/led.h"
#include "MCAL/UART/uart.h"


//#define CMD_READ_DTC    'w'  // Read current DTC
//#define CMD_CLEAR_DTC   'e'  // Clear DTC
//#define CMD_READ_SENSOR 'r'  // Read live sensor data (e.g., temperature)
//#define CMD_TEST_GPIO   't'  // Toggle a GPIO for testing
//#define CMD_EXIT_MODE   'y'  // Exit Tester Mode

#define CMD_READ_DTC    '1'  // Read current DTC
#define CMD_CLEAR_DTC   '2'  // Clear DTC
#define CMD_READ_SENSOR '3'  // Read live sensor data (e.g., temperature)
#define CMD_TEST_GPIO_ECU2              '4'
#define CMD_TEST_GPIO_ECU1              '5'
#define CMD_EXIT_MODE   '6'  // Exit Tester Mode

static uint32_t OS_ui8OverheatDTCCounter;
static uint32_t g_DTC;
extern bool OS_boolDTCFlag;
extern bool OS_boolVoltageDTCFlag;
extern bool OS_boolCommunicationDTCFlag;
// EEPROM Addresses
#define EEPROM_ERROR_COUNTER_ADDR   0x100  // Offset for error counter
#define EEPROM_DTC_ADDR             0x200  // Offset for DTC
#define EEPROM_BUTTON_COUNTER_ADDR  0x300  // EEPROM address for the button counter
#define EEPROM_COMM_COUNTER_ADDR    0x400  // EEPROM address for the button counter
#define EEPROM_COMM_DTC_ADDR        0x500  // Offset for DTC
#define MOTOR_OVERHEAT_TIME            3   // Overheat time in seconds

// Threshold for entering fail-safe state
#define ERROR_THRESHOLD 3

// Function Prototypes
void initializeEEPROM(void);
//void incrementErrorCounter(void);
void resetFault(void);
void failSafeState(void);
void APP_ui32CheckButtons(void);
void NVM_voidIncrementDTCCounter(void);
void checkButtonPresses(void);
void checkFaultFlag(void);
void NVM_voidIncrementCommunicationDTCCounter(void);

//void checkButtonPressesForReset(void);

//void checkMotorOverheat(bool isOverheated);
uint32_t readDTC(void);
void clearDTC(void);

// Function prototypes
void EEPROM_Init(void);
void EEPROM_Write(uint32_t address, uint32_t data);
uint32_t EEPROM_Read(uint32_t address);

#endif
