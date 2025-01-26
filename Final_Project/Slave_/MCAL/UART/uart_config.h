#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

/**
 * UART Module Selection:
 *      UART_MODULE_0  // Use UART0
 *      UART_MODULE_1  // Use UART1
 *      UART_MODULE_2  // Use UART2
 */
#define UART_MODULE                UART_MODULE_0

/**
 * GPIO Configuration for UART0:
 */
#define UART0_PORT                 SYSCTL_PERIPH_GPIOA
#define UART0_RX_PIN               GPIO_PIN_0
#define UART0_TX_PIN               GPIO_PIN_1
#define UART0_RX_CFG               GPIO_PA0_U0RX
#define UART0_TX_CFG               GPIO_PA1_U0TX

/**
 * GPIO Configuration for UART1:
 */
#define UART1_PORT                 SYSCTL_PERIPH_GPIOB
#define UART1_RX_PIN               GPIO_PIN_0
#define UART1_TX_PIN               GPIO_PIN_1
#define UART1_RX_CFG               GPIO_PB0_U1RX
#define UART1_TX_CFG               GPIO_PB1_U1TX

/**
 * Select GPIO ports, pins, and UART base based on the selected UART_MODULE.
 */
#if UART_MODULE == UART_MODULE_0
    #define UART_SYSCTL_PERIPH      SYSCTL_PERIPH_UART0
    #define UART_GPIO_PORT          UART0_PORT
    #define UART_GPIO_RX_PIN        UART0_RX_PIN
    #define UART_GPIO_TX_PIN        UART0_TX_PIN
    #define UART_GPIO_RX_CFG        UART0_RX_CFG
    #define UART_GPIO_TX_CFG        UART0_TX_CFG
    #define UART_BASE               UART0_BASE
    #define UART_GPIO_BASE          GPIO_PORTA_BASE
#else
    #define UART_SYSCTL_PERIPH      SYSCTL_PERIPH_UART1
    #define UART_GPIO_PORT          UART1_PORT
    #define UART_GPIO_RX_PIN        UART1_RX_PIN
    #define UART_GPIO_TX_PIN        UART1_TX_PIN
    #define UART_GPIO_RX_CFG        UART1_RX_CFG
    #define UART_GPIO_TX_CFG        UART1_TX_CFG
    #define UART_BASE               UART1_BASE
    #define UART_GPIO_BASE          GPIO_PORTB_BASE
#endif

// **UART Baud Rate**
/**
 * Options for Baud Rate:
 *      9600       // 9600 bps
 *      19200      // 19200 bps
 *      38400      // 38400 bps
 *      115200     // 115200 bps
 */
#define UART_BAUD_RATE            9600

// **UART Word Length**
/**
 * Options for Word Length:
 *      UART_CONFIG_WLEN_5   // 5 data bits
 *      UART_CONFIG_WLEN_6   // 6 data bits
 *      UART_CONFIG_WLEN_7   // 7 data bits
 *      UART_CONFIG_WLEN_8   // 8 data bits
 */
#define UART_WORD_LENGTH          UART_CONFIG_WLEN_8

// **UART Parity Configuration**
/**
 * Options for Parity:
 *      UART_CONFIG_PAR_NONE  // No parity
 *      UART_CONFIG_PAR_EVEN  // Even parity
 *      UART_CONFIG_PAR_ODD   // Odd parity
 */
#define UART_PARITY               UART_CONFIG_PAR_NONE

// **UART Stop Bits Configuration**
/**
 * Options for Stop Bits:
 *      UART_CONFIG_STOP_ONE  // 1 Stop Bit
 *      UART_CONFIG_STOP_TWO  // 2 Stop Bits
 */
#define UART_STOP_BITS            UART_CONFIG_STOP_ONE

#endif /* UART_CONFIG_H_ */
