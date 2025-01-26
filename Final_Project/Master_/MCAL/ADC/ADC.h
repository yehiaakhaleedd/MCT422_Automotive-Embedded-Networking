#ifndef ADC_H_
#define ADC_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"

#define ADC_MAX_VALUE 4095.0
#define TEMP_MIN 0.0    // Minimum temperature in °C
#define TEMP_MAX 40.0    // Maximum temperature in °C

#define VOLTAGE_MAX 3.0           // Desired maximum voltage
#define VOLTAGE_MIN 0.0           // Minimum voltage (0V)

void initADC(void);
void readADCValue(uint32_t *adc_value);
float mapADCToTemperature(uint32_t adc_value);
float getTemperature(void);
void readADC1Value(uint32_t *adc_value);
void initADC1(void);
float getVoltage(void);
uint32_t testADC1_ReadValue(void);


#endif /* ADC_H_ */








//The Main.c
/*
#include "ADC.h"

int main(void) {
    uint32_t adc_value;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    initADC();

    while (1) {
        readADCValue(&adc_value);

        if (adc_value >= 2048)
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); // Turn off green LED
        else
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on green LED
    }
}

 */









//The APP
/*
int main(void)
{
    uint32_t adc_value;

    // Enable Clock to ADC0 and GPIO pins

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // Enable Clock to GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);   // AD0 clock enable

    // Initialize PE3 for AIN0 input

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);  // Enable PE3 as ADC input

    // Initialize sample sequencer3

    ADCSequenceDisable(ADC0_BASE, 3);  // Disable SS3 during configuration
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Software trigger conversion
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END); // Get input from channel 0
    ADCSequenceEnable(ADC0_BASE, 3);   // Enable ADC0 sequencer 3

    // Initialize PF3 as a digital output pin

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // Enable Clock to GPIOF
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); // Set GREEN pin as a digital output pin

    while (1)
    {
        ADCIntClear(ADC0_BASE, 3);  // Clear ADC interrupt status
        ADCProcessorTrigger(ADC0_BASE, 3);  // Enable SS3 conversion or start sampling data from AN0
        while (!ADCIntStatus(ADC0_BASE, 3, false)); // Wait until sample conversion completed
        ADCSequenceDataGet(ADC0_BASE, 3, &adc_value); // Read ADC conversion result from SS3 FIFO

        // Control Green PF3->LED

        if (adc_value >= 2048)
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); // Turn on green LED
        else
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn off green LED
    }
}
*/


//Trial with Interrupt
/*
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"

volatile uint32_t ui32Value;

void ADCIntHandler(void) {
    ADCIntClear(ADC0_BASE, 0);  // Clear ADC interrupt status
    ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value); // Read ADC conversion result

    // Toggle the LED based on the ADC threshold (2000)
    if (ui32Value > 2000) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on LED (PF3)
    } else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Turn off LED (PF3)
    }
}

int main(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  // Enable ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Enable GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable GPIOF

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)); // Wait for ADC0 to be ready

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // Enable PE3 as ADC input

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntRegister(ADC0_BASE, 0, ADCIntHandler); // Register ADC interrupt handler
    ADCIntEnable(ADC0_BASE, 0); // Enable ADC interrupt
    IntEnable(INT_ADC0SS0); // Enable ADC sequence 0 interrupt
    IntMasterEnable(); // Enable master interrupts

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); // Configure PF3 as output

    ADCProcessorTrigger(ADC0_BASE, 0); // Trigger the sample sequence

    while (1) {
        // Your main code logic goes here
        // You can perform other tasks here or simply wait in a low-power mode
    }
}
*/


//2nd
/*
#include "ADC.h"

int main(void) {
    uint32_t adc_value;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    initADC();

    while (1) {
        readADCValue(&adc_value);

        if (adc_value >= 2048)
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); // Turn off green LED
        else
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn on green LED
    }
}
*/

