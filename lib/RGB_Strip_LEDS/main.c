/*  Includes  */
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
#include <servo_api.h>
#include <servo_api.c>
//#include <lcd_api.h>
#define DEBUG
#define ENTER_CHAR 'F'
#define FREQUENCY   1000 
#define SPEED       5
//#define DEMO        //This actives the demo to test the strip
#define RED(r)      PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,r*(Period/255))
#define GREEN(g)    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,g*(Period/255))
#define BLUE(b)     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,b*(Period/255))

/*  Global variables  */
volatile uint32_t CharUART;
uint8_t index = 0, 
        PWM_RED = 0,
        PWM_GREEN = 0,
        PWM_BLUE = 0,
        flag = 0,
        buffer[]={"C000000000"}; //#R#G#B

uint16_t Period;

void UARTIntHandler(void)
{
  uint32_t ui32Status,hand;
  ui32Status = UARTIntStatus(UART0_BASE, 1); //get interrupt status
  UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

  while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
  {
    CharUART=UARTCharGet(UART0_BASE);
    if((CharUART >= 48 && CharUART <= 57)|| CharUART == 'C' || CharUART == ENTER_CHAR) 
    { 
      #ifdef  DEBUG
        UARTprintf("%c",CharUART);
      #endif
      switch(index)
      {
        case 0:
          if(CharUART=='C' && !flag)
          {
            buffer[index] = CharUART;
            index++;
          }
        break;
        case 1:
          buffer[index] = CharUART;
          index++;
        break;
        case 2:
          buffer[index] = CharUART;
          index++;
        break;
        case 3:
          buffer[index] = CharUART;
          index++;
        break;
        case 4:
          buffer[index] = CharUART;
          index++;
        break;
        case 5:
          buffer[index] = CharUART;
          index++;
        break;
        case 6:
          buffer[index] = CharUART;
          index++;
        break;
        case 7:
          buffer[index] = CharUART;
          index++;
        break;
        case 8:
          buffer[index] = CharUART;
          index++;
        break;
        case 9:
          buffer[index] = CharUART;
          index++;
        break;
        case 10:
          if(CharUART == ENTER_CHAR)
          {
            index = 0;
            PWMCalc(&buffer);
          }
        break;
      }
    } 
    else if(CharUART == 'A') 
    {
      int aux;
      for(aux=0;aux<10;aux++)
        buffer[aux] = 0;
      index = 0;
    }
  }
}

void PWMCalc(char* pwm_value)
{
  PWM_RED = (*(pwm_value+1)-0x30)*100+(*(pwm_value+2)-0x30)*10+(*(pwm_value+3)-0x30);
  PWM_GREEN = (*(pwm_value+4)-0x30)*100+(*(pwm_value+5)-0x30)*10+(*(pwm_value+6)-0x30);
  PWM_BLUE = (*(pwm_value+7)-0x30)*100+(*(pwm_value+8)-0x30)*10+(*(pwm_value+9)-0x30);
  RED(PWM_RED);
  GREEN(PWM_GREEN);
  BLUE(PWM_BLUE);

  #ifdef  DEBUG
    UARTprintf("\nPWM RED:%d",PWM_RED);
    UARTprintf("\nPWM GREEN:%d",PWM_GREEN);
    UARTprintf("\nPWM BLUE:%d",PWM_BLUE);
    UARTprintf("\nInsert value(CRGB):");
  #endif
}

void CfgModUART()
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
  UARTStdioConfig(0, 115200, 16000000);

  IntMasterEnable(); //enable processor interrupts

  IntEnable(INT_UART0); //enable the UART interrupt

  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
}

void CfgClock()
{
  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //40MHz
}

void delay_ms(long delayms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

void CfgPWM()
{  
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  
  //N = (1 / f) * SysClk
  Period = SysCtlClockGet()/FREQUENCY;
  //Period = 40000;

  GPIOPinConfigure(GPIO_PB6_M0PWM0);
  GPIOPinConfigure(GPIO_PB7_M0PWM1);
  GPIOPinConfigure(GPIO_PB4_M0PWM2);

  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_4);
 
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0 , PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1 , PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0 , Period);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1 , Period);

  PWMGenEnable(PWM0_BASE,PWM_GEN_0);
  PWMGenEnable(PWM0_BASE,PWM_GEN_1);
  PWMOutputState(PWM0_BASE,PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT, true);
 
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,Period/2);
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,Period/2);
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,Period/2);
}

void main(void)
{
	int r,g,b;
  CfgClock();
  CfgPWM();
  CfgModUART();
  
  #ifdef  DEBUG
    UARTprintf("\n\nHello World RGB!");
    UARTprintf("\nInsert value(C-RGB-F):");
  #endif
  while(1)
  {
    #ifdef DEMO
      for(r = 0; r < 256; r++) 
      { 
        RED(r);
        delay_ms(SPEED);
      } 

      for(b = 255; b > 0; b--) 
      { 
        BLUE(b);
        delay_ms(SPEED);
      } 

      for(g = 0; g < 256; g++) 
      { 
        GREEN(g);
        delay_ms(SPEED);
      } 

      for(r = 255; r > 0; r--) 
      { 
        RED(r);
        delay_ms(SPEED);
      } 

      for(b = 0; b < 256; b++) 
      { 
        BLUE(b);
        delay_ms(SPEED);
      } 

      for(g = 255; g > 0; g--) 
      { 
        GREEN(g);
        delay_ms(SPEED);
      } 
    #else
      //RED(25);
      //GREEN(25);
      //BLUE(112);
    #endif
  }
}


