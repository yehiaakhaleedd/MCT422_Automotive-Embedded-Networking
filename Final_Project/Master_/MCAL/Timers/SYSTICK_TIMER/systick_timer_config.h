#ifndef SYSTICK_TIMER_CONFIG_H_
#define SYSTICK_TIMER_CONFIG_H_

#include "systickTimer.h"

/***********************************************
 * SysTick Configuration Options
 ***********************************************/

// Select the desired SysTick interval (choose one of the options below):
// Options:
// SYSTICK_INTERVAL_1MS    - Generate an interrupt every 1 millisecond
// SYSTICK_INTERVAL_10MS   - Generate an interrupt every 10 milliseconds
// SYSTICK_INTERVAL_50MS   - Generate an interrupt every 50 milliseconds
// SYSTICK_INTERVAL_100MS  - Generate an interrupt every 100 milliseconds
// SYSTICK_INTERVAL_500MS  - Generate an interrupt every 500 milliseconds
// SYSTICK_INTERVAL_1S     - Generate an interrupt every 1 second
#define SYSTICK_TICK_INTERVAL_MS            SYSTICK_INTERVAL_1MS

// Enable or disable SysTick interrupts (choose one of the options below):
// Options:
// SYSTICK_INTERRUPT_ENABLE  - Enable SysTick interrupts
// SYSTICK_INTERRUPT_DISABLE - Disable SysTick interrupts
#define SYSTICK_INTERRUPT_MODE              SYSTICK_INTERRUPT_ENABLE
#endif /* SYSTICK_TIMER_CONFIG_H_ */
