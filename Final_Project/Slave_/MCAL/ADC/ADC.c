#include "ADC.h"

void initADC(void) {
    // Enable Clock to GPIOE and ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure PE3 as ADC input
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // Disable sequencer 3 during configuration
    ADCSequenceDisable(ADC0_BASE, 3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Software trigger
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END); // Channel 0
    ADCSequenceEnable(ADC0_BASE, 3);   // Enable ADC0 sequencer 3
}

void readADCValue(uint32_t *adc_value) {
    // Clear ADC interrupt status and trigger conversion
    ADCIntClear(ADC0_BASE, 3);
    ADCProcessorTrigger(ADC0_BASE, 3);

    // Wait for conversion to complete
    while (!ADCIntStatus(ADC0_BASE, 3, false));

    // Retrieve ADC conversion result from SS3 FIFO
    ADCSequenceDataGet(ADC0_BASE, 3, adc_value);
}

float mapADCToTemperature(uint32_t adc_value) {
    // Map ADC value to temperature range
    return ((float)adc_value / ADC_MAX_VALUE) * (TEMP_MAX - TEMP_MIN) + TEMP_MIN;
}

float getTemperature(void) {
    uint32_t adc_value;

    // Clear ADC interrupt status and trigger conversion
    ADCIntClear(ADC0_BASE, 3);
    ADCProcessorTrigger(ADC0_BASE, 3);

    // Wait for conversion to complete
    while (!ADCIntStatus(ADC0_BASE, 3, false));

    // Retrieve ADC conversion result from SS3 FIFO
    ADCSequenceDataGet(ADC0_BASE, 3, &adc_value);

    // Map ADC value to temperature
    return ((float)adc_value / ADC_MAX_VALUE) * (TEMP_MAX - TEMP_MIN) + TEMP_MIN;
}

float getVoltage(void) {
    uint32_t adc_value;

    // Clear ADC interrupt status and trigger conversion
    ADCIntClear(ADC1_BASE, 3);
    ADCProcessorTrigger(ADC1_BASE, 3);

    // Wait for conversion to complete
    while (!ADCIntStatus(ADC1_BASE, 3, false));

    // Retrieve ADC conversion result from SS3 FIFO
    ADCSequenceDataGet(ADC1_BASE, 3, &adc_value);

    // Normalize ADC value to 0V–3V range
    return ((float)adc_value / ADC_MAX_VALUE) * (VOLTAGE_MAX - VOLTAGE_MIN) + VOLTAGE_MIN;
}
void initADC1(void) {
    // Enable Clock to GPIOE and ADC1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // GPIO for ADC1 input
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);  // ADC1 module

    // Configure PE2 as ADC input (ADC1 Channel 1)
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

    // Disable sequencer 3 during configuration
    ADCSequenceDisable(ADC1_BASE, 3);

    // Configure Sequencer 3 for software-triggered conversion
    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    // Configure the first step of sequencer 3 to read from channel 1 (PE2)
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    // Enable sequencer 3
    ADCSequenceEnable(ADC1_BASE, 3);

    // Clear any pending interrupts
    ADCIntClear(ADC1_BASE, 3);
}

void readADC1Value(uint32_t *adc_value) {
    // Clear ADC interrupt status and trigger conversion
    ADCIntClear(ADC1_BASE, 3);
    ADCProcessorTrigger(ADC1_BASE, 3);

    // Wait for conversion to complete
    while (!ADCIntStatus(ADC1_BASE, 3, false));

    // Retrieve ADC conversion result from SS3 FIFO
    ADCSequenceDataGet(ADC1_BASE, 3, adc_value);
}

