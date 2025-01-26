#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"

/**
 * Options for SPI Module Selection:
 *      SPI_MODULE_0   // Use SSI0
 *      SPI_MODULE_1   // Use SSI1
 */
#define SPI_MODULE                  SPI_MODULE_1

/**
 * Options for SPI Module Selection:
 *      SSI_MODE_MASTER
 *      SSI_MODE_SLAVE
 */
#define SPI_MODE                    SSI_MODE_SLAVE


/**
 * Define GPIO ports, pins, and pin configurations for SPI0
 */
#define SPI0_PORT                   SYSCTL_PERIPH_GPIOA   // GPIO port for SSI0
#define SPI0_SCLK_PIN               GPIO_PIN_2           // SCLK pin for SSI0
#define SPI0_FSS_PIN                GPIO_PIN_3           // Frame Select pin for SSI0
#define SPI0_MISO_PIN               GPIO_PIN_4           // MISO pin for SSI0
#define SPI0_MOSI_PIN               GPIO_PIN_5           // MOSI pin for SSI0
#define SPI0_SCLK_CFG               GPIO_PA2_SSI0CLK     // Pin configuration for SSI0 SCLK
#define SPI0_FSS_CFG                GPIO_PA3_SSI0FSS     // Pin configuration for SSI0 Frame Select
#define SPI0_MISO_CFG               GPIO_PA4_SSI0RX      // Pin configuration for SSI0 MISO
#define SPI0_MOSI_CFG               GPIO_PA5_SSI0TX      // Pin configuration for SSI0 MOSI
#define SPI0_PORT_BASE              GPIO_PORTA_BASE
#define SPI0_BASE                   SSI0_BASE
#define SPI0_INT                    INT_SSI0

/**
 * Define GPIO ports, pins, and pin configurations for SPI1
 */
#define SPI1_PORT                   SYSCTL_PERIPH_GPIOD   // GPIO port for SSI1
#define SPI1_SCLK_PIN               GPIO_PIN_0           // SCLK pin for SSI1
#define SPI1_FSS_PIN                GPIO_PIN_1           // Frame Select pin for SSI1
#define SPI1_MISO_PIN               GPIO_PIN_2           // MISO pin for SSI1
#define SPI1_MOSI_PIN               GPIO_PIN_3           // MOSI pin for SSI1
#define SPI1_SCLK_CFG               GPIO_PD0_SSI1CLK     // Pin configuration for SSI1 SCLK
#define SPI1_FSS_CFG                GPIO_PD1_SSI1FSS     // Pin configuration for SSI1 Frame Select
#define SPI1_MISO_CFG               GPIO_PD2_SSI1RX      // Pin configuration for SSI1 MISO
#define SPI1_MOSI_CFG               GPIO_PD3_SSI1TX      // Pin configuration for SSI1 MOSI
#define SPI1_PORT_BASE              GPIO_PORTD_BASE
#define SPI1_BASE                   SSI1_BASE
#define SPI1_INT                    INT_SSI1

/**
 * Automatically select GPIO ports, pins, and SPI base based on the SPI_MODULE
 */
#if SPI_MODULE == SPI_MODULE_0
    #define SPI_SYSCTL_PERIPH       SPI0_PORT            // Peripheral for SSI0
    #define SPI_GPIO_PORT_BASE      SPI0_PORT_BASE       // GPIO port base for SSI0
    #define SPI_GPIO_SCLK_PIN       SPI0_SCLK_PIN        // SCLK pin for SSI0
    #define SPI_GPIO_FSS_PIN        SPI0_FSS_PIN         // Frame Select pin for SSI0
    #define SPI_GPIO_MISO_PIN       SPI0_MISO_PIN        // MISO pin for SSI0
    #define SPI_GPIO_MOSI_PIN       SPI0_MOSI_PIN        // MOSI pin for SSI0
    #define SPI_GPIO_SCLK_CFG       SPI0_SCLK_CFG        // SCLK pin configuration for SSI0
    #define SPI_GPIO_FSS_CFG        SPI0_FSS_CFG         // Frame Select pin configuration for SSI0
    #define SPI_GPIO_MISO_CFG       SPI0_MISO_CFG        // MISO pin configuration for SSI0
    #define SPI_GPIO_MOSI_CFG       SPI0_MOSI_CFG        // MOSI pin configuration for SSI0
    #define SPI_BASE                SPI0_BASE            // Base address for SSI0
    #define SPI_INT                 SPI0_INT             // Interrupt number for SSI0
#else
    #define SPI_SYSCTL_PERIPH       SPI1_PORT            // Peripheral for SSI1
    #define SPI_GPIO_PORT_BASE      SPI1_PORT_BASE       // GPIO port base for SSI1
    #define SPI_GPIO_SCLK_PIN       SPI1_SCLK_PIN        // SCLK pin for SSI1
    #define SPI_GPIO_FSS_PIN        SPI1_FSS_PIN         // Frame Select pin for SSI1
    #define SPI_GPIO_MISO_PIN       SPI1_MISO_PIN        // MISO pin for SSI1
    #define SPI_GPIO_MOSI_PIN       SPI1_MOSI_PIN        // MOSI pin for SSI1
    #define SPI_GPIO_SCLK_CFG       SPI1_SCLK_CFG        // SCLK pin configuration for SSI1
    #define SPI_GPIO_FSS_CFG        SPI1_FSS_CFG         // Frame Select pin configuration for SSI1
    #define SPI_GPIO_MISO_CFG       SPI1_MISO_CFG        // MISO pin configuration for SSI1
    #define SPI_GPIO_MOSI_CFG       SPI1_MOSI_CFG        // MOSI pin configuration for SSI1
    #define SPI_BASE                SPI1_BASE            // Base address for SSI1
    #define SPI_INT                 SPI1_INT             // Interrupt number for SSI1
#endif

/**
 * Options for Clock Frequency (in Hz):
 *      SPI_CLOCK_1MHZ            // 1 MHz
 *      SPI_CLOCK_500KHZ          // 500 KHz
 */
#define SPI_CLOCK_FREQUENCY         1000000

/**
 * Options for Data Width:
 *      SPI_DATA_WIDTH_8             // 8 bits per transfer
 *      SPI_DATA_WIDTH_16            // 16 bits per transfer
 */
#define SPI_DATA_WIDTH              8

/**
 * Options for SPI Frame Format:
 *      SPI_FRAME_FORMAT_MOTO        // Motorola SPI Frame Format
 *      SPI_FRAME_FORMAT_TI          // TI Synchronous Serial Frame Format
 *      SPI_FRAME_FORMAT_NMW         // National MicroWire Frame Format
 */
#define SPI_FRAME_FORMAT            SSI_FRF_MOTO_MODE_0

#endif /* SPI_CONFIG_H_ */
