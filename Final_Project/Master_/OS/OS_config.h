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
#define configUSE_SPI                        1
#define configUSE_TIMER0                     1
#define configUSE_TIMER1                     0
#define configUSE_SYSTICK_TIMER              1
#define configUSE_UART                       1
#define configUSE_EEPROM                     1

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

#define MAX_TASKS 10
TaskControlBlock taskList[MAX_TASKS];

#endif /* CAN_CONFIG_H_ */
