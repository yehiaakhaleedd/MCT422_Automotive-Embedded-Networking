/*
 * uart.c
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: The purpose of this file is to provide functionality for UART communication on the microcontroller, specifically using UART0.
 *               It initializes UART0 to communicate at 9600 baud, sets up GPIO pins for UART functionality,and enables sending and receiving data via UART.
 *               Additionally, the file includes functions to send messages over UART, and output specific state messages.
 */


/***********************************************
 * Includes
 ***********************************************/
#include <MCAL/uart.h>



/***********************************************
 * Functions Definitions
 ***********************************************/

/***********************************************
 * Function Name: UART0_Init
 * Inputs: N/A
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Initializes the UART0 peripheral for communication at 9600 baud rate with 8 data bits, no parity, and 1 stop bit.
 *              It also configures the necessary GPIO pins (PA0, PA1) for UART RX and TX, sets up the system clock, and configures PF1 as
 *              an output for controlling a Red LED. The function also sends an initial message ("start") to a terminal via UART if it has
 *              not been sent previously. It ensures proper initialization of UART0 and delays for the setup to complete before starting communication.
 ***********************************************/
void UART0_init(void) {

    // Set the clock for the system (50MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable the GPIO peripheral for Port A (for UART0) and Port F (for the Red LED)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  // For UART0 (PA0, PA1)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // For Red LED (PF1)

    // Enable the UART0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure PA0 for UART0 RX and PA1 for UART0 TX
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Set UART0 for 9600 baud rate, 8 data bits, no parity, 1 stop bit
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE));
    UARTEnable(UART0_BASE);

    // Configure PF1 as output for the Red LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    // Turn off the Red LED initially
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    // Delay to allow UART initialization
    SysCtlDelay(SysCtlClockGet() / 3);

    // Set a flag to indicate whether the initial message has been sent
    initialMessageSent = false;
    // Send an initial message only once
    if (!initialMessageSent)
    {
        // Send a message to PuTTY
        UART0_sendMessage("start\r\n");

        // Set initialMessageSent to true to avoid sending the message repeatedly
        initialMessageSent = true;
    }

}



/***********************************************
 * Function Name: UARTSendMessage
 * Inputs: const char *array_ptr - A pointer to a null-terminated string to be transmitted over UART.
 * Outputs: N/A
 * Reentrancy: Non-Reentrant
 * Synchronous: Synch
 * Description: Sends each character of the input string (`array_ptr`) one by one over the UART0 interface. The function transmits the
 *              string by iterating through each character and using the `UARTCharPut` function to transmit each character to the UART0
 *              base until the null-terminator is reached.
 ***********************************************/
void UART0_sendMessage(const char *array_ptr)
{
    while (*array_ptr)
    {
        UARTCharPut(UART0_BASE, *array_ptr);  // Transmit each character
        array_ptr++;
    }
}

