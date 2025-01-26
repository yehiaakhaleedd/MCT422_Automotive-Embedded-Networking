#ifndef OS_CONFIG_H_
#define OS_CONFIG_H_

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "OS/scheduler.h"



#define configUSE_LEDS                       1
#define configUSE_BUTTONS                    1
#define configUSE_CAN                        1
#define configUSE_SPI                        0
#define configUSE_TIMER0                     1
#define configUSE_TIMER1                     0
#define configUSE_SYSTICK_TIMER              1
#define configUSE_UART                       1

//// =============================================
//// Scheduler Configuration
//// =============================================
//#define configMAX_TASK_COUNT                 10      // Maximum number of tasks
//#define configTICK_RATE_MS                   1       // Tick rate in milliseconds
//
//// =============================================
//// Task Priorities
//// =============================================
//#define PRIORITY_HIGH                        1       // Lowest numerical value = highest priority
//#define PRIORITY_MEDIUM                      2
//#define PRIORITY_LOW                         3
//
//// =============================================
//// Task Periods (in OS ticks)
//// =============================================
//#define LED_BLINK_PERIOD_TICKS               500     // Example: Blink LED every 500 ticks
//#define BUTTON_CHECK_PERIOD_TICKS            100     // Example: Check buttons every 100 ticks
//#define UART_TASK_PERIOD_TICKS               200     // Example: Send UART message every 200 ticks
//#define CAN_RECEIVE_PERIOD_TICKS             50      // Example: Poll CAN messages every 50 ticks

// =============================================
// Task Configuration (Define Task Functions)
// =============================================

// Task list (to be initialized in OS_Config.c)
//extern TaskConfig taskList[];

typedef enum {
    TASK_READY,
    TASK_WAITING,
    TASK_RUNNING
} TaskState;

typedef struct {
    void (*taskFunction)(void);
    uint32_t periodTicks;
    uint32_t lastRunTick;
    uint8_t priority;
    TaskState state;
    uint8_t isEnabled;           // Task enabled/disabled
} TaskControlBlock;

#define MAX_TASKS 0x10U
TaskControlBlock taskList[MAX_TASKS];

#endif /* CAN_CONFIG_H_ */
