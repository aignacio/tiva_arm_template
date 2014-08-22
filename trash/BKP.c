/****************************************************************************

    Demo using AD and PWM - 0 channel
    Author: Anderson Ignacio da Silva
    Date: 19/03/2014
    Inf.: http://www.esplatforms.blogspot.com.br

****************************************************************************/


#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/debug.h>
#include <driverlib/pwm.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <inc/hw_gpio.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/adc.h>
#include <driverlib/uart.h>
#include <driverlib/rom.h>
#include <utils/uartstdio.h>
#include <utils/uartstdio.c>
#include <lcd_api.h>
#include <driverlib/timer.h>

//#include <ad_keyboard_api.h>
//#include <ad_api.h>

extern void lcd_init();
//extern void cfg_adc(void);
//extern void ADCIntHandler(void);

#define PWM_FREQUENCY 55
//#define DEBUG

uint16_t ulPeriod,dutyCycle;

typedef struct{
uint32_t Cycle1;
uint32_t Cycle2;
uint32_t Cycle3;
uint32_t Cycle4;
uint32_t Period;
}Duty_t;


Duty_t Duty;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("Error at line %d of %s\n", ui32Line, pcFilename);
    while(1)
    {
    }
}
#endif

void delay_ms(long delayms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

void UARTIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, 1); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE)); //echo character
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
        delay_ms(20);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
    }
}

void cfgClock()
{
  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //40MHz
}

void enPeriph()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED
}

void cfgGPIO()
{
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //enable pin for LED PF2

}

void cfgUART()
{
    //UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.

   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

   //
   // Configure the pin muxing for UART0 functions on port A0 and A1.
   // This step is not necessary if your part does not support pin muxing.
   // TODO: change this to select the port/pin you are using.
   //
   GPIOPinConfigure(GPIO_PA0_U0RX);
   GPIOPinConfigure(GPIO_PA1_U0TX);

   //
   // Enable UART0 so that we can configure the clock.
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

   //
   // Select the alternate (UART) function for these pins.
   // TODO: change this to select the port/pin you are using.
   //
   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

   //
   // Initialize the UART for console I/O.
   //
   UARTStdioConfig(0, 115200, 16000000);
}

void cfgInts()
{
    IntMasterEnable(); //enable processor interrupts

    IntEnable(INT_UART0); //enable the UART interrupt

    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
}

void cfgPWM()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable port F
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable port B
  GPIOPinConfigure(GPIO_PF2_T1CCP0); // Configure pin PF2 as output of Timer 1_A
  GPIOPinConfigure(GPIO_PF3_T1CCP1); // Configure pin PF3 as output of Timer 1_B
  GPIOPinConfigure(GPIO_PB6_T0CCP0); // Configure pin PB6 as output of Timer 0_A
  GPIOPinConfigure(GPIO_PB7_T0CCP1); // Configure pin PB7 as output of Timer 0_B
  GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2 ); // Enable pin PF2 as output of timer addressed to it
  GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_3 ); // Enable pin PF3 as output of timer addressed to it
  GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6 ); // Enable pin PB6 as output of timer addressed to it
  GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_7 ); // Enable pin PB7 as output of timer addressed to it

  Duty.Period = (SysCtlClockGet() / 500)/2;
  Duty.Cycle1 = (unsigned long)(Duty.Period-1)*0.8;
  Duty.Cycle2 = (unsigned long)(Duty.Period-1)*0.6;
  Duty.Cycle3 = (unsigned long)(Duty.Period-1)*0.4;
  Duty.Cycle4 = (unsigned long)(Duty.Period-1)*0.2;


  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // Enable Timer 1
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Enable Timer 0
  TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM)); // Configure Timer 1 as two 16 but timers with both functioning as PWM
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM)); // Configure Timer 0 as two 16 but timers with both functioning as PWM
  TimerControlLevel(TIMER1_BASE, TIMER_BOTH, 0); //  Timer 1 is trigger low
  TimerControlLevel(TIMER0_BASE, TIMER_BOTH, 0); // Timer 0 is trigger low
  TimerLoadSet(TIMER1_BASE, TIMER_A, Duty.Period -1); // Timer 1 Load set
  TimerLoadSet(TIMER1_BASE, TIMER_B, Duty.Period -1);
  TimerLoadSet(TIMER0_BASE, TIMER_A, Duty.Period -1); // Timer 0 Load set
  TimerLoadSet(TIMER0_BASE, TIMER_B, Duty.Period -1);
  TimerMatchSet(TIMER1_BASE, TIMER_A, Duty.Cycle1); // Timer 1 Match set
  TimerMatchSet(TIMER1_BASE, TIMER_B, Duty.Cycle2);
  TimerMatchSet(TIMER0_BASE, TIMER_A, Duty.Cycle3); // Timer 0 Match set
  TimerMatchSet(TIMER0_BASE, TIMER_B, Duty.Cycle4);
  TimerEnable(TIMER1_BASE, TIMER_BOTH);
  TimerEnable(TIMER0_BASE, TIMER_BOTH);
}

int main(void)
{
  FPUEnable();
  FPUStackingEnable();

  cfgClock();
  enPeriph();
  cfgInts();
  cfgGPIO();
  cfgUART();
  cfgPWM();


  lcd_init();
  goto_lcd(1,1);
  //lcd_string("Teste_4");

  while(1) 
  {
	  Duty.Cycle1 += 0.001;
	  delay_ms(500);
  }    //UARTprintf("Teste...");
}
