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
#include <MCAL/UART/uart.h>
#include "MCAL/UART/uart_config.h"



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
void UART_Init(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    // Enable the UART module and associated GPIO port
    SysCtlPeripheralEnable(UART_SYSCTL_PERIPH);
    SysCtlPeripheralEnable(UART_GPIO_PORT);

    while (!SysCtlPeripheralReady(UART_SYSCTL_PERIPH)) {}
    while (!SysCtlPeripheralReady(UART_GPIO_PORT)) {}

    // Configure GPIO pins for UART RX and TX
    GPIOPinConfigure(UART_GPIO_RX_CFG);
    GPIOPinConfigure(UART_GPIO_TX_CFG);
    GPIOPinTypeUART(UART_GPIO_BASE, UART_GPIO_RX_PIN | UART_GPIO_TX_PIN);

    // Set UART0 for 9600 baud rate, 8 data bits, no parity, 1 stop bit
    UARTConfigSetExpClk(UART_BASE, SysCtlClockGet(), UART_BAUD_RATE, (UART_WORD_LENGTH | UART_PARITY | UART_STOP_BITS));
    UARTEnable(UART_BASE);

//    // Enable the GPIO peripheral for the selected UART
//    if (UART_MODULE == UART_MODULE_0) {
//        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  // For UART0 (PA0, PA1)
//        GPIOPinConfigure(GPIO_PA0_U0RX);
//        GPIOPinConfigure(GPIO_PA1_U0TX);
//        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    }
//    else if (UART_MODULE == UART_MODULE_1) {
//        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // For UART1 (PB0, PB1)
//        GPIOPinConfigure(GPIO_PB0_U1RX);
//        GPIOPinConfigure(GPIO_PB1_U1TX);
//        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    }
//    else{}
//
//    // Enable the UART peripheral
//    if (UART_MODULE == UART_MODULE_0) {
//        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//    }
//    else if (UART_MODULE == UART_MODULE_1) {
//        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//    }
//    else{}

    // Delay to allow UART initialization
    //SysCtlDelay(SysCtlClockGet() / 3);

    // Set a flag to indicate whether the initial message has been sent
    initialMessageSent = false;
    // Send an initial message only once
    if (!initialMessageSent)
    {
        // Send a message to PuTTY
        UART_SendMessage("start\r\n");

        // Set initialMessageSent to true to avoid sending the message repeatedly
        initialMessageSent = true;
    }

}

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
void UART_SendMessage(const char *message) {
    while (*message) {
        UARTCharPut(UART_BASE, *message);  // Send character
        message++;
    }
}

void UART0_sendMessage(const char *array_ptr)
{
    while (*array_ptr)
    {
        UARTCharPut(UART0_BASE, *array_ptr);  // Transmit each character
        array_ptr++;
    }
}

void UART_SendNumber(uint8_t number) {
    char buffer[12];  // Buffer to hold the string representation of the number (max 10 digits + sign + null terminator)
    int index = 0;
    int i = 0;
    // Handle special case if number is 0
    if (number == 0) {
        buffer[index++] = '0';
    } else {
        // Convert the number to a string (reverse order)
        while (number > 0) {
            buffer[index++] = (number % 10) + '0';  // Convert the last digit to a character
            number /= 10;  // Remove the last digit
        }
    }

    buffer[index] = '\0';  // Null-terminate the string

    // Reverse the buffer to get the correct order
    for (i = 0; i < index / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[index - 1 - i];
        buffer[index - 1 - i] = temp;
    }

    // Send the string using UART
    UART_SendMessage(buffer);  // Assuming UART_SendMessage is your function to send strings
}

