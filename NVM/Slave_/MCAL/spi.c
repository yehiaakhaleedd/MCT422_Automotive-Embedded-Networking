/*
 * spi.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to configure and manage communication between a master and slave using the SPI (Serial Peripheral Interface) protocol,
 *      specifically using SSI1 (Synchronous Serial Interface). It provides initialization functions for both the master and slave configurations
 *      and includes functionality for sending data from the master to the slave and receiving an acknowledgment.
 */


/***********************************************
 * Includes
 ***********************************************/
#include "spi.h"


/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: SPI_masterInit
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the SPI (SSI1) module in master mode. The function sets the system clock to 50MHz, enables
 *              the necessary peripherals for the SPI (SSI1) and GPIO Port D, and waits for the peripherals to be ready.
 *              It then configures the pins for SPI communication (SCLK, SS, MISO, MOSI) on GPIO Port D and sets them
 *              to SSI mode. The function finally configures the SPI module for 1 MHz frequency, 8 data bits per transfer,
 *              and Motorola SPI mode 0, and enables the SPI module for communication.
 ***********************************************/
void SPI_masterInit(void)
{

    // Set the clock to 50MHz
        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

        // Enable the peripherals for SPI (SSI1) and GPIO Port D
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        // Wait for peripherals to be ready
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI1) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {}

        // Configure pins for SPI (SSI1)
        GPIOPinConfigure(GPIO_PD0_SSI1CLK);   // SCLK
        GPIOPinConfigure(GPIO_PD1_SSI1FSS);   // SS (Slave Select)
        GPIOPinConfigure(GPIO_PD2_SSI1RX);    // MISO
        GPIOPinConfigure(GPIO_PD3_SSI1TX);    // MOSI
        GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

        // Configure the SPI module
        SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, 1000000, 8);

        // Enable the SPI module
        SSIEnable(SSI1_BASE);
}



/***********************************************
 * Function Name: SPI_slaveInit
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the SPI (SSI1) module in slave mode. The function first sets the system clock to 50 MHz and
 *              enables the necessary peripherals for the SPI (SSI1) and GPIO Port D. It waits for the peripherals to be
 *              ready, then configures the pins for SPI communication (SCLK, Frame Select, MISO, MOSI) on GPIO Port D.
 *              The function then sets up the SPI module in slave mode, using Motorola SPI mode 0, with a clock frequency
 *              of 1 MHz and 8 data bits per transfer. Finally, the SPI module is enabled for slave communication.
 ***********************************************/
void SPI_slaveInit(void)
{
    // Set up system clock at 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


    // Enable SSI1 and Port D peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI1) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {}

    // Configure SPI pins for SSI1
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);   // SCLK
    GPIOPinConfigure(GPIO_PD1_SSI1FSS);   // Frame Select
    GPIOPinConfigure(GPIO_PD2_SSI1RX);    // MISO (Master In Slave Out)
    GPIOPinConfigure(GPIO_PD3_SSI1TX);    // MOSI (Master Out Slave In)
    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Configure SSI1 as SPI slave, mode 0, 1 MHz, 8-bit data
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_SLAVE, 1000000, 8);
    SSIEnable(SSI1_BASE);  // Enable SSI1
}



/***********************************************
 * Function Name: SPI_masterSend
 * Inputs: uint32_t a_uint32counter - The data to be sent through SPI
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: This function sends a 32-bit data value (a_uint32counter) via SPI from the master
 *              device. It controls the Slave Select (FSS) pin to signal the start and
 *              end of communication and ensures the transmission completes before
 *              releasing the FSS pin.
 ***********************************************/
void SPI_masterSend(uint32_t a_uint32counter)
{
    //uint32_t CommLostCount = 0;
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);  // Set FSS low
    SSIDataPut(SSI1_BASE, a_uint32counter);                    // Send data
    while(SSIBusy(SSI1_BASE)) {}                    // Wait for transmission to complete
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);  // Set FSS high
}

/***********************************************
 * Function Name: SPI_checkData
 * Inputs: N/A
 * Outputs: bool - Returns true if data is received, false if no data is available
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Checks if SPI data is available to be read. Attempts to read data
 *              from the SPI bus using a non-blocking call and returns true if data
 *              is successfully received, otherwise returns false.
 ***********************************************/
bool SPI_checkData(void)
{
    // Attempt to read SPI data
    if (SSIDataGetNonBlocking(SSI1_BASE, &receivedData)) {
        return true; // Data was successfully received
    }
    return false; // No data available
}


/***********************************************
 * Function Name: SPI_putData
 * Inputs: uint32_t a_uint32data - The 32-bit data to be sent via SPI
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Sends the provided 32-bit data through the SPI interface by
 *              writing it to the SSI1 data register.
 ***********************************************/
void SPI_putData(uint32_t a_uint32data)
{
    SSIDataPut(SSI1_BASE, a_uint32data);
}



