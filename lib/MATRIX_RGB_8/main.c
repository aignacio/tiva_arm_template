/*  Includes  */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "inc/hw_timer.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include <utils/uartstdio.h>
#include <utils/uartstdio.c>
#include <driverlib/uart.h>
#include "pattern.h"
#include "matrix_8_rgb.h"

#include "driverlib/ssi.h"

#define LINE_PINS       GPIO_PIN_5+GPIO_PIN_6+GPIO_PIN_7
#define LINE_PORT       GPIO_PORTA_BASE

#define LAT_PIN         GPIO_PIN_5
#define LAT_PORT        GPIO_PORTE_BASE

#define BLANK_PIN       GPIO_PIN_5
#define BLANK_PORT      GPIO_PORTB_BASE

#define SCLK_PIN        GPIO_PIN_4
#define SCLK_PORT       GPIO_PORTB_BASE

#define MOSI_PIN        GPIO_PIN_7
#define MOSI_PORT       GPIO_PORTB_BASE

#define REPEAT_MATRIX_UNTIL_INCREMENT 10 //Repeat 10 times until change image in the matrix

#define set_high(x,y)     GPIOPinWrite(x,y,y)
#define set_low(x,y)      GPIOPinWrite(x,y,~(y)) 
#define set_custom(x,y,z) GPIOPinWrite(x,y,z) 

/*  Global variables  */
volatile uint32_t CharUART;
bool              flag = 0;

volatile uint16_t increment_offset=0,
                  time=0;
volatile uint8_t  repeat_screen=0;

void 
UARTIntHandler(void)
{
  uint32_t ui32Status;
  ui32Status = UARTIntStatus(UART0_BASE, 1); //get interrupt status
  UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
  while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
  {
    CharUART=UARTCharGet(UART0_BASE);
    UARTprintf("%c",CharUART);
  }
}


//*****************************************************************************
//
// The interrupt handler for the Timer0A interrupt.
//
//*****************************************************************************
void
Timer0IntHandler(void)
{
  TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

  //toggle_led();
  if(increment_offset > 3071) increment_offset -= 3072;

  if(SendSSITLC(pattern, increment_offset))
   repeat_screen++;

  if(repeat_screen == REPEAT_MATRIX_UNTIL_INCREMENT) 
  {
    repeat_screen = 0;
    increment_offset += 24;
    if(increment_offset > 3071) 
      increment_offset = 0;
  }
}


void
toggle_led()
{
  if(flag)
  {
    flag = 0;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_PIN_3);
  }
  else
  {
    flag = 1;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,~GPIO_PIN_3);
  }
}

void 
CfgClock(void)
{
  //SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //40MHz
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void 
delay_ms(long delayms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

void 
CfgModUART(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

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
  UARTStdioConfig(0, 57600, 16000000);

  IntMasterEnable(); //enable processor interrupts

  IntEnable(INT_UART0); //enable the UART interrupt

  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
}


void 
CfgTimer()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/10); //1second/10=100miliseconds
  IntMasterEnable();
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  IntEnable(INT_TIMER0A);
  TimerEnable(TIMER0_BASE, TIMER_A);
}

void 
main(void)
{
  CfgClock();
  CfgModUART();
  CfgMatrixPins();
  CfgTimer();

  while(1)
  {
    //UARTprintf("AT+GMR\n\r");
    //delay_ms(1200);
  };
}
