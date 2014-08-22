/****************************************************************************
    ███████╗   ███████╗   ██████╗
    ██╔════╝   ██╔════╝   ██╔══██╗
    █████╗     ███████╗   ██████╔╝
    ██╔══╝     ╚════██║   ██╔═══╝
    ███████╗██╗███████║██╗██║
    ╚══════╝╚═╝╚══════╝╚═╝╚═╝

    SERVO MOTOR - API
    Author: Anderson Ignacio da Silva
    Date: 08/08/2014
    Target:PART_TM4C123GH6PM
    Inf.: http://www.esplatforms.blogspot.com.br

    Obs.:

****************************************************************************/

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
//#include <lcd_api.h>

/*  Definitions       */
#define PWM_FREQUENCY   50
#define PWM_PERIOD      20000  //Time in us
#define MAX_SERVO_TIME  2300   //Time for max PWM servo (us)  //TMÁX=2100 & TMIN=500 FOR MG995
#define MIN_SERVO_TIME  800    //Time for min PWM servo (us)    //TMÁX=2300 & TMIN=800 FOR HS422
#define DEBUG

/*  Global variables  */
volatile uint32_t Cperiod, IntervalDuty, CharUART, degree, aux, min_c,max_c;
volatile uint8_t  CharSel[3]={'000'},CharVal[3]={'000'},inc=0,aux_sel,aux_val;
volatile bool     RefreshUART=0,sel=0;

/*  Prototypes  */

/*  Functions */
void CfgModGPIO()
{
  // ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Enable clock for GPIOF
  // /* Unlock pull ups to buttons on board */
  // HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;  
  // HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
  // HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
  // /* Unlock pull ups to buttons on board */
  // ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);  //Configure as input the PORTF_0 and PORTF_4
  // ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //Configure as input with fan in of 2mA, weak pull-up
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}

void UARTIntHandler(void)
{
  uint32_t ui32Status;
  ui32Status = UARTIntStatus(UART0_BASE, 1); //get interrupt status
  UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
  while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
  {
    CharUART=UARTCharGet(UART0_BASE);
    if((CharUART>=48 && CharUART<=57)||CharUART == 32 || CharUART == 13)
    {
      UARTprintf("%c",CharUART);
      if(CharUART == 32)
      { 
        switch(inc)
        {
          case 0:
            aux_sel = 0;
          break;
          case 1: 
            aux_sel = (CharSel[0]-48);
          break; 
          case 2:
            aux_sel = ((CharSel[1]-48)+(CharSel[0]-48)*10);
          break;
          case 3:
            aux_sel = ((CharSel[0]-48)*100+(CharSel[1]-48)*10+(CharSel[2]-48));
          break;
          default:
            aux_sel = 0;
          break; 
        }
        inc = 0;
        sel = 1;
      }
      else  if(CharUART != 13)
            {
              if(!sel)
              {
                if(inc<3)
                {
                  CharSel[inc] = CharUART;
                  inc++;
                }
                else
                {
                  for(aux_sel=0;aux_sel<3;aux_sel++) CharSel[aux_sel] = '0';
                  for(aux_val=0;aux_val<3;aux_val++) CharVal[aux_val] = '0'; 
                  RefreshUART = 0;
                  inc = 0;
                  sel = 0;
                  UARTprintf("\nCaracteres excedem o limite maximo!");
                }
              }
              else
              {
                if(inc<3)
                {
                  CharVal[inc] = CharUART;
                  inc++;
                }
                else
                {
                  for(aux_sel=0;aux_sel<3;aux_sel++) CharSel[aux_sel] = '0';
                  for(aux_val=0;aux_val<3;aux_val++) CharVal[aux_val] = '0'; 
                  RefreshUART = 0;
                  inc = 0;
                  sel = 0;
                  UARTprintf("\nCaracteres excedem o limite maximo!");
                }
              }
            }
            else
            {
              switch(inc)
              {
                case 0:
                  aux_val = 0;
                break;
                case 1: 
                   aux_val = (CharVal[0]-48);
                break; 
                case 2:
                   aux_val = ((CharVal[0]-48)*10+(CharVal[1]-48));
                break;
                case 3:
                  aux_val = ((CharVal[0]-48)*100+(CharVal[1]-48)*10+(CharVal[2]-48));
                break;
                default:
                  aux_val = 0;
                break; 
              }
              SendServo(aux_sel, aux_val);
              inc = 0;
              sel = 0;
              for(aux_sel=0;aux_sel<3;aux_sel++) CharSel[aux_sel] = '0';
              for(aux_val=0;aux_val<3;aux_val++) CharVal[aux_val] = '0'; 
              RefreshUART = 0;
              aux_val = 0;
              aux_sel = aux_val;
            }
    }
    else
    {
      UARTprintf("\nCaracter Invalido");
      RefreshUART = 0;
    }
    // //echo character
    
    // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
    // delay_ms(20);
    // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
    // Switch
    // SendServo();
  }
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

void SendServo(uint8_t SelServo,uint8_t ValServo)
{ 
  aux = (ValServo*degree+min_c);
  #ifdef DEBUG
    UARTprintf("\n\nVALOR aux:%d",aux);
  #endif
  if(aux < min_c) aux = min_c;
  else if(aux > 92000) aux = max_c;
  IntervalDuty = (Cperiod - aux);
  
  #ifdef DEBUG
    UARTprintf("\n\nSERVO:%d,VALOR:%d",SelServo,IntervalDuty);
  #endif

  switch(SelServo)
  {
    case 1:
      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER0_BASE, TIMER_A,IntervalDuty);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 2:
      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER0_BASE, TIMER_B,IntervalDuty);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 3:
      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER1_BASE, TIMER_A,IntervalDuty);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 4:
      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER1_BASE, TIMER_B,IntervalDuty);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 5:
      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER2_BASE, TIMER_A,IntervalDuty);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");    
    break;
    case 6:
      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER2_BASE, TIMER_B,IntervalDuty);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");  
    break;
    default:
      UARTprintf("\n\rServo não reconhecido!");
    break;
  }
  //delay_ms(15);
}

void CfgModPWM()
{ 
  Cperiod=(SysCtlClockGet()/PWM_FREQUENCY);

  /*********************CONFIG_PWM_1 && 2*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  GPIOPinConfigure(GPIO_PC4_WT0CCP0);
  GPIOPinConfigure(GPIO_PC5_WT0CCP1);
  GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
  GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
  TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
  
  TimerLoadSet(WTIMER0_BASE, TIMER_A,Cperiod);
  TimerMatchSet(WTIMER0_BASE, TIMER_A,Cperiod-(Cperiod*MIN_SERVO_TIME)/PWM_PERIOD);
  TimerEnable(WTIMER0_BASE, TIMER_A);

  TimerLoadSet(WTIMER0_BASE, TIMER_B, Cperiod);
  TimerMatchSet(WTIMER0_BASE, TIMER_B, Cperiod-(Cperiod*MAX_SERVO_TIME)/PWM_PERIOD);
  TimerEnable(WTIMER0_BASE, TIMER_B);
  /*********************CONFIG_PWM_1 && 2*********************************/

  /*********************CONFIG_PWM_3 && 4*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
  GPIOPinConfigure(GPIO_PC6_WT1CCP0);
  GPIOPinConfigure(GPIO_PC7_WT1CCP1);
  GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_6);
  GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_7);
  TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
  
  TimerLoadSet(WTIMER1_BASE, TIMER_A,Cperiod);
  TimerMatchSet(WTIMER1_BASE, TIMER_A,Cperiod-(Cperiod*MIN_SERVO_TIME)/PWM_PERIOD);
  TimerEnable(WTIMER1_BASE, TIMER_A);

  TimerLoadSet(WTIMER1_BASE, TIMER_B, Cperiod);
  TimerMatchSet(WTIMER1_BASE, TIMER_B, Cperiod-(Cperiod*MAX_SERVO_TIME)/PWM_PERIOD);
  TimerEnable(WTIMER1_BASE, TIMER_B);
  /*********************CONFIG_PWM_3 && 4*********************************/

  /*********************CONFIG_PWM_5 && 6*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  GPIOPinConfigure(GPIO_PD0_WT2CCP0);
  GPIOPinConfigure(GPIO_PD1_WT2CCP1);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
  TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
  
  TimerLoadSet(WTIMER2_BASE, TIMER_A,Cperiod);
  TimerMatchSet(WTIMER2_BASE, TIMER_A,Cperiod-(Cperiod*MIN_SERVO_TIME)/PWM_PERIOD);
  TimerEnable(WTIMER2_BASE, TIMER_A);

  TimerLoadSet(WTIMER2_BASE, TIMER_B, Cperiod);
  TimerMatchSet(WTIMER2_BASE, TIMER_B, Cperiod-(Cperiod*MAX_SERVO_TIME)/PWM_PERIOD);
  TimerEnable(WTIMER2_BASE, TIMER_B);
  /*********************CONFIG_PWM_5 && 6*********************************/
}

void calc_servo()
{   
  min_c = (Cperiod*MIN_SERVO_TIME)/PWM_PERIOD;
  max_c = (Cperiod*MAX_SERVO_TIME)/PWM_PERIOD;

  //Rule of three proportional
  IntervalDuty=(max_c-min_c);
  degree = (IntervalDuty / 180);
} 

void CfgClock()
{
  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //40MHz
}

void delay_ms(long delayms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

int main(void)
{ 
  uint8_t i;

  CfgClock();
  CfgModUART();
  CfgModPWM();
  CfgModGPIO();
  calc_servo();

  //for(i=1;i<=6;i++) SendServo(i,0);

  while(1) 
  {
    if(!RefreshUART)
    {
      RefreshUART = 1;
      UARTprintf("\nDigite os valores de saida dos motores:");
    }
  }
}


