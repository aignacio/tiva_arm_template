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


    Obs.:Com 6 saídas PWM, o mcu trava, talvez por falta de espaço na memória de dados
         Para gerar as 6 saídas terá que ser feito um timer de 1uS e contadores individuais
         para cada saída de PWM.

****************************************************************************/

/*  Includes  */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include <utils/uartstdio.h>
#include <utils/uartstdio.c> 
#include <driverlib/uart.h>
#include <driverlib/timer.h> 
//#include "inc/tm4c123gh6pm.h"       
//#include <lcd_api.h>

/*  Definitions       */
#define PWM_FREQUENCY   50
#define PWM_PERIOD      20000  //Time in us
#define MAX_SERVO_TIME  2300   //Time for max PWM servo (us)  //TMÁX=2100 & TMIN=500 FOR MG995
#define MIN_SERVO_TIME  800  //Time for min PWM servo (us)    //TMÁX=2300 & TMIN=800 FOR HS422
#define DEBUG
/*  Global variables  */
volatile uint32_t ui32Load, ui32PWMClock, CharUART,degree;
volatile uint16_t ui16Adjust,min_c,max_c;
volatile uint8_t  Servo_1=1;
volatile bool     RefreshUART=0,sel=0;
volatile uint8_t  CharSel[3]={'000'},CharVal[3]={'000'},inc=0,aux_sel,aux_val;
/*  Prototypes  */

/*  Functions */
void delay_ms(long delayms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

void cfg_PWM()
{ 
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //Configure clock to 40MHz
  ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);  //Configure PWM clock to 625kHz (40.000.000/64)
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); //Enable PWM_0 output
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); //Enable PWM_1 output
  
  ui32PWMClock = SysCtlClockGet() / 64;  //Configure clock for PWM - 50Hz (ESCs and Servo motor default)
  ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;


  /*********************CONFIG_PWM_1*********************************/
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Enable PWM_0/0 output
  ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6); //Set PB6 for PWM output
  ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);  //Configure PB6 as PWM_0/0 output
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ui32Load);
  ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,1);  //MÁX=1562 MIN=937 - 55Hz (1.5ms----2.5ms)
  /*********************END_CONFIG_PWM_1*****************************/

  /*********************CONFIG_PWM_2*********************************/
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Enable PWM_0/1 output
  ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7); //Set PB7 for PWM output
  ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);  //Configure PB7 as PWM_0/1 output
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ui32Load);
  ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,1);  //MÁX=1562 MIN=937 - 55Hz (1.5ms----2.5ms)
  /*********************END_CONFIG_PWM_2*****************************/

  /*********************CONFIG_PWM_3*********************************/
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Enable PWM_0/2 output
  ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4); //Set PB4 for PWM output
  ROM_GPIOPinConfigure(GPIO_PB4_M0PWM2);  //Configure PB7 as PWM_0/2 output
  PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, ui32Load);
  ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,1);  //MÁX=1562 MIN=937 - 55Hz (1.5ms----2.5ms)
  /*********************END_CONFIG_PWM_3*****************************/

  /*********************CONFIG_PWM_4*********************************/
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Enable PWM_0/3 output
  ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5); //Set PB5 for PWM output
  ROM_GPIOPinConfigure(GPIO_PB5_M0PWM3);  //Configure PB5 as PWM_0/3 output
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, ui32Load);
  ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,1);  //MÁX=1562 MIN=937 - 55Hz (1.5ms----2.5ms)
  /*********************END_CONFIG_PWM_4*****************************/

  /*********************CONFIG_PWM_5*********************************/
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); //Enable PWM_1/0 output
  ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0); //Set PD0 for PWM output
  ROM_GPIOPinConfigure(GPIO_PD0_M1PWM0);  //Configure PD0 as PWM_1/0 output
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);
  ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,ui32Load/2);  //MÁX=1562 MIN=937 - 55Hz (1.5ms----2.5ms)
  /*********************END_CONFIG_PWM_5*****************************/

  /*********************CONFIG_PWM_6*********************************/
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); //Enable PWM_1/1 output
  ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1); //Set PD1 for PWM output
  ROM_GPIOPinConfigure(GPIO_PD1_M1PWM1);  //Configure PD1 as PWM_1/1 output
  PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, ui32Load);
  ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1,ui32Load/2);  //MÁX=1562 MIN=937 - 55Hz (1.5ms----2.5ms)
  /*********************END_CONFIG_PWM_6*****************************/

  ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
  ROM_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
  ROM_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
  ROM_PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
  ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
  ROM_PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);

  ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
  ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
  ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
  ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_3);
  ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);
  ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_1);
}

void cfg_GPIO()
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Enable clock for GPIOF
  /* Unlock pull ups to buttons on board */
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;  
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
  /* Unlock pull ups to buttons on board */
  ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);  //Configure as input the PORTF_0 and PORTF_4
  ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //Configure as input with fan in of 2mA, weak pull-up
}

void read_pb_pwm(uint16_t *temp_min,uint16_t *temp_max)
{
  if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
  {
    ui16Adjust--;
    if (ui16Adjust < *temp_min)
    {
      ui16Adjust = *temp_min;
    }
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, ui16Adjust);
  }
  if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
  {
    ui16Adjust++;
    if (ui16Adjust > *temp_max)
    {
      ui16Adjust = *temp_max;
    }
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui16Adjust);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, ui16Adjust);
  }
   //UARTprintf("\n\rValor do PWM:%d",ui16Adjust);
  //delay_ms(1);
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

void cfgUART()
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
  ui16Adjust=(degree*ValServo+min_c*10);
  ui16Adjust=ui16Adjust/10;
  if(ui16Adjust < min_c) ui16Adjust = min_c;
  else if(ui16Adjust > max_c) ui16Adjust = max_c;
  
  #ifdef DEBUG
    UARTprintf("\n\nSERVO:%d,VALOR:%d",SelServo,ui16Adjust);
  #endif

  switch(SelServo)
  {
    case 1:
      if(ValServo >= 0 && ValServo <= 180)
      {
        ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ui16Adjust);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 2:
      if(ValServo >= 0 && ValServo <= 180)
      {
        ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ui16Adjust);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 3:
      if(ValServo >= 0 && ValServo <= 180)
      {
        ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ui16Adjust);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 4:
      if(ValServo >= 0 && ValServo <= 180)
      {
        ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ui16Adjust);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");
    break;
    case 5:
      if(ValServo >= 0 && ValServo <= 180)
      {
        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui16Adjust);
      } 
      else
        UARTprintf("\n\rValor excedente aos limites!");    
    break;
    case 6:
      if(ValServo >= 0 && ValServo <= 180)
      {
        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, ui16Adjust);
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

void calc_servo()
{   
  uint16_t aux;

  ui32PWMClock = SysCtlClockGet() / 64;  //COnfigure clock for PWM - 50Hz (ESCs and Servo motor default)
  ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
  min_c=((ui32Load*MIN_SERVO_TIME)/PWM_PERIOD);
  max_c=((ui32Load*MAX_SERVO_TIME)/PWM_PERIOD);

  //Rule of three proportional
  aux=(max_c*10-min_c*10);
  degree = (aux / 180);
} 

void cfg_Timer()
{
  uint32_t Period=800000/*(40MHz/800.000=50Hz)*/,DutyCycle=Period/2;
  
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);   //40MHz
  
  /*********************CONFIG_PWM_1*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  GPIOPinConfigure(GPIO_PC4_WT0CCP0);
  GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
  TimerConfigure(WTIMER0_BASE,  TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
  TimerLoadSet(WTIMER0_BASE, TIMER_A, Period);
  TimerMatchSet(WTIMER0_BASE, TIMER_A, DutyCycle);
  TimerEnable(WTIMER0_BASE, TIMER_A);
  /*********************CONFIG_PWM_1*********************************/

  /*********************CONFIG_PWM_2*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
  //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  GPIOPinConfigure(GPIO_PC6_WT1CCP0);
  GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_6);
  TimerConfigure(WTIMER1_BASE,  TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
  TimerLoadSet(WTIMER1_BASE, TIMER_A, Period);
  TimerMatchSet(WTIMER1_BASE, TIMER_A, DutyCycle);
  TimerEnable(WTIMER1_BASE, TIMER_A);
  /********************CONFIG_PWM_2********************************

  /*********************CONFIG_PWM_3*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  GPIOPinConfigure(GPIO_PD0_WT2CCP0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
  TimerConfigure(WTIMER2_BASE,  TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
  TimerLoadSet(WTIMER2_BASE, TIMER_A, Period);
  TimerMatchSet(WTIMER2_BASE, TIMER_A, DutyCycle);
  TimerEnable(WTIMER2_BASE, TIMER_A);
  /*********************CONFIG_PWM_3*********************************/

  /*********************CONFIG_PWM_4*********************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
  //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  GPIOPinConfigure(GPIO_PD2_WT3CCP0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_2);
  TimerConfigure(WTIMER3_BASE,  TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
  TimerLoadSet(WTIMER3_BASE, TIMER_A, Period);
  TimerMatchSet(WTIMER3_BASE, TIMER_A, DutyCycle);
  TimerEnable(WTIMER3_BASE, TIMER_A);
  /*********************CONFIG_PWM_4*********************************/
}

// void blink_status()
// {
//   volatile uint32_t ui32Loop;

//   //
//   // Enable the GPIO port that is used for the on-board LED.
//   //
//   SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

//   //
//   // Do a dummy read to insert a few cycles after enabling the peripheral.
//   //
//   ui32Loop = SYSCTL_RCGC2_R;

//   //
//   // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
//   // enable the GPIO pin for digital function.
//   //
//   GPIO_PORTF_DIR_R = 0x08;
//   GPIO_PORTF_DEN_R = 0x08;

//   //
//   // Loop forever.
//   //
//   while(1)
//   {
//   //
//   // Turn on the LED.
//   //
//   GPIO_PORTF_DATA_R |= 0x08;

//   //
//   // Delay for a bit.
//   //
//   for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
//   {
//   }

//   //
//   // Turn off the LED.
//   //
//   GPIO_PORTF_DATA_R &= ~(0x08);

//   //
//   // Delay for a bit.
//   //
//   for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
//   {
//   }
//   }
// }

int main(void)
{ 
  uint16_t i;

  //cfg_PWM();
  cfgUART();
  cfg_Timer();
  cfg_GPIO();
  //calc_servo();

  while(1)
  {
    if(!RefreshUART)
    {
      RefreshUART = 1;
      UARTprintf("\nDigite os valores de saida dos motores:");
    }
  }
}
