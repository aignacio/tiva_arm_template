/****************************************************************************

    ███████╗   ███████╗   ██████╗
    ██╔════╝   ██╔════╝   ██╔══██╗
    █████╗     ███████╗   ██████╔╝
    ██╔══╝     ╚════██║   ██╔═══╝
    ███████╗██╗███████║██╗██║
    ╚══════╝╚═╝╚══════╝╚═╝╚═╝

    AD Keyboard API - API for use the AD channel-0 of tiva C series microcontroles
    Author: Anderson Ignacio da Silva
    Date: 04/07/2014
    Target:PART_TM4C123GH6PM
    Inf.: http://www.esplatforms.blogspot.com.br


****************************************************************************/

#ifndef SYSCTL_H
#define SYSCTL_H
	#include "driverlib/sysctl.h"
#endif
#ifndef ADC_H
#define ADC_H
	#include "driverlib/adc.h"
#endif
#ifndef GPIO_H
#define GPIO_H
	#include "driverlib/gpio.h"
#endif

//Handler of interrupt that controls the analog to digital conversion
void ADCIntHandler(void)
{
	uint32_t	adc_ch[1];
    //uint8_t     loop;

	ADCIntClear(ADC0_BASE, 3);

	// Read ADC Value.
	ADCSequenceDataGet(ADC0_BASE, 3, adc_ch);
    
    // switch(adc_ch[0])
    // {
    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;

    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;

    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;

    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;
    //     case:
    //     break;
        
    //     default:
    //     break;
    // }

    UARTprintf("                                            ");
	UARTprintf("\rAD value of channel 1-PIN PE2:%d",adc_ch[0]);
}

void cfg_adc()
{
    //
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // For this example ADC0 is used with AIN0 on port E7.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.  GPIO port E needs to be enabled
    // so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Select the analog ADC function for these pins.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // information on the ADC sequences and steps, reference the datasheet.
    //
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
	ADCIntClear(ADC0_BASE, 3);

	ADCIntRegister(ADC0_BASE, 3, *ADCIntHandler);

	ADCIntEnable(ADC0_BASE, 3);
}
