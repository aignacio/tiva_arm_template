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

    PWM Pins:
    Channels  |   Pin
       0          PC4
       1          PC5
       2          PC6
       3          PC7
       4          PD0
       5          PD1

    Instructions:
        -To initialize just call InitServo()
        -Change the duty with SendServo(Channel,0-180 degrees)
        -If your servo has a different duty period, you can ch
         ange the time definitions in "servo_api.h"
        -Remember to change the MIN and MAX duty of each servo

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
#include "servo_api.h"
/*  Global variables  */
volatile uint32_t Cperiod, 
                  IntervalDuty[6], 
                  degree[6],
                  aux, 
                  min_c[6],
                  max_c[6];

volatile uint16_t servo_times[12];

/*
  SendServo - Envia sinal para cada servo motor individualmente
  
  Parâmetros de entrada: 
    -uint8_t SelServo - Seleciona servo a ser controlado (0-5)
    -uint8_t ValServo - Valor que será enviado para o servo motor (0-180)
  
  Parâmetros de saída:
    -Nenhum

  Protótipo:
  void SendServo(uint8_t SelServo,uint8_t ValServo)
*/
void SendServo(uint8_t SelServo,uint8_t ValServo)
{  
  #ifdef DEBUG_SERVO
    UARTprintf("\n\nSERVO:%d,VALOR:%d",SelServo,ValServo);
  #endif

  switch(SelServo)
  {
    case 0:
      aux = (ValServo*degree[SelServo]+min_c[SelServo]);
      if(aux < min_c[SelServo]) aux = min_c[SelServo];
      else if(aux > (max_c[SelServo]+min_c[SelServo])) aux = max_c[SelServo];
      IntervalDuty[SelServo] = (Cperiod - aux);

      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER0_BASE, TIMER_A,IntervalDuty[SelServo]);
      } 
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rValor excedente aos limites!");
    #endif
    break;
    case 1:
      aux = (ValServo*degree[SelServo]+min_c[SelServo]);
      if(aux < min_c[SelServo]) aux = min_c[SelServo];
      else if(aux > (max_c[SelServo]+min_c[SelServo])) aux = max_c[SelServo];
      IntervalDuty[SelServo] = (Cperiod - aux);

      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER0_BASE, TIMER_B,IntervalDuty[SelServo]);
      } 
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rValor excedente aos limites!");
    #endif
    break;
    case 2:
      aux = (ValServo*degree[SelServo]+min_c[SelServo]);
      if(aux < min_c[SelServo]) aux = min_c[SelServo];
      else if(aux > (max_c[SelServo]+min_c[SelServo])) aux = max_c[SelServo];
      IntervalDuty[SelServo] = (Cperiod - aux);

      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER1_BASE, TIMER_A,IntervalDuty[SelServo]);
      } 
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rValor excedente aos limites!");
    #endif
    break;
    case 3:
      aux = (ValServo*degree[SelServo]+min_c[SelServo]);
      if(aux < min_c[SelServo]) aux = min_c[SelServo];
      else if(aux > (max_c[SelServo]+min_c[SelServo])) aux = max_c[SelServo];
      IntervalDuty[SelServo] = (Cperiod - aux);

      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER1_BASE, TIMER_B,IntervalDuty[SelServo]);
      } 
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rValor excedente aos limites!");
    #endif
    break;
    case 4:
      aux = (ValServo*degree[SelServo]+min_c[SelServo]);
      if(aux < min_c[SelServo]) aux = min_c[SelServo];
      else if(aux > (max_c[SelServo]+min_c[SelServo])) aux = max_c[SelServo];
      IntervalDuty[SelServo] = (Cperiod - aux);

      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER2_BASE, TIMER_A,IntervalDuty[SelServo]);
      } 
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rValor excedente aos limites!");
    #endif
    break;
    case 5:
      aux = (ValServo*degree[SelServo]+min_c[SelServo]);
      if(aux < min_c[SelServo]) aux = min_c[SelServo];
      else if(aux > (max_c[SelServo]+min_c[SelServo])) aux = max_c[SelServo];
      IntervalDuty[SelServo] = (Cperiod - aux);

      if(ValServo >= 0 && ValServo <= 180)
      {
        TimerMatchSet(WTIMER2_BASE, TIMER_B,IntervalDuty[SelServo]);
      } 
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rValor excedente aos limites!");
    #endif
    break;
    default:
    #ifdef DEBUG_SERVO
      else
        UARTprintf("\n\rServo não reconhecido!");
    #endif
    break;
  }
  //delay_ms(15);
}

/*
  CfgModPWM - Configura os canais de PWM dos servo motores
  
  Parâmetros de entrada: 
    -Nenhum
  
  Parâmetros de saída:
    -Nenhum
    
  Protótipo:
  void CfgModPWM()
*/
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
  TimerMatchSet(WTIMER0_BASE, TIMER_A,Cperiod-(Cperiod*MIN_SERVO_0)/PWM_PERIOD);
  TimerEnable(WTIMER0_BASE, TIMER_A);

  TimerLoadSet(WTIMER0_BASE, TIMER_B, Cperiod);
  TimerMatchSet(WTIMER0_BASE, TIMER_B, Cperiod-(Cperiod*MIN_SERVO_1)/PWM_PERIOD);
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
  TimerMatchSet(WTIMER1_BASE, TIMER_A,Cperiod-(Cperiod*MIN_SERVO_2)/PWM_PERIOD);
  TimerEnable(WTIMER1_BASE, TIMER_A);

  TimerLoadSet(WTIMER1_BASE, TIMER_B, Cperiod);
  TimerMatchSet(WTIMER1_BASE, TIMER_B, Cperiod-(Cperiod*MIN_SERVO_3)/PWM_PERIOD);
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
  TimerMatchSet(WTIMER2_BASE, TIMER_A,Cperiod-(Cperiod*MIN_SERVO_4)/PWM_PERIOD);
  TimerEnable(WTIMER2_BASE, TIMER_A);

  TimerLoadSet(WTIMER2_BASE, TIMER_B, Cperiod);
  TimerMatchSet(WTIMER2_BASE, TIMER_B, Cperiod-(Cperiod*MIN_SERVO_5)/PWM_PERIOD);
  TimerEnable(WTIMER2_BASE, TIMER_B);
  /*********************CONFIG_PWM_5 && 6*********************************/
}

/*
  CalcServo - Configura a resolução de cada canal PWM
  
  Parâmetros de entrada: 
    -Nenhum
  
  Parâmetros de saída:
    -Nenhum
    
  Protótipo:
  void CalcServo()
*/
void CalcServo() 
{  
  uint8_t i,j;

  for(i=0,j=0;i<=5;i++,j+2) 
  {
    max_c[i] = (Cperiod*servo_times[j])/PWM_PERIOD;
    min_c[i] = (Cperiod*servo_times[j+1])/PWM_PERIOD;
    IntervalDuty[i]=(max_c[i]-min_c[i]);
    #ifdef DEBUG_SERVO
      UARTprintf("\nServo:%d,Max:%d",i,max_c[i]);
      UARTprintf("\nMAX_DEFINE:%d",servo_times[j]);
      UARTprintf("\nServo:%d,Min:%d",i,min_c[i]);
      UARTprintf("\nMIN_DEFINE:%d",servo_times[j+1]);   
      UARTprintf("\nInterval duty:%d",IntervalDuty[i]);
    #endif
    degree[i] = (IntervalDuty[i]/180);
    j=(j+2);
  }
} 

/*
  InitServo - Inicializa a configuração dos servo motores
  
  Parâmetros de entrada: 
    -Nenhum
  
  Parâmetros de saída:
    -Nenhum
    
  Protótipo:
  void CalcServo()
*/
void InitServo()
{
    uint8_t i;
    servo_times[0]=MAX_SERVO_0;
    servo_times[1]=MIN_SERVO_0;

    servo_times[2]=MAX_SERVO_1;
    servo_times[3]=MIN_SERVO_1;
    
    servo_times[4]=MAX_SERVO_2;
    servo_times[5]=MIN_SERVO_2;
    
    servo_times[6]=MAX_SERVO_3;
    servo_times[7]=MIN_SERVO_3;

    servo_times[8]=MAX_SERVO_4;
    servo_times[9]=MIN_SERVO_4;

    servo_times[10]=MAX_SERVO_5;
    servo_times[11]=MIN_SERVO_5;

    CfgModPWM();
    CalcServo();
    for(i=0;i<=5;i++) SendServo(i,0);
}

/*********************************************************************************USED TO DEBUG THE MOTORS THROUGH UART*********************************************************************************/
// void CfgModUART()
// {
//   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

//   GPIOPinConfigure(GPIO_PA0_U0RX);
//   GPIOPinConfigure(GPIO_PA1_U0TX);
//   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

//   //UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

//   // Enable GPIO port A which is used for UART0 pins.
//   // TODO: change this to whichever GPIO port you are using.

//   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

//   //
//   // Configure the pin muxing for UART0 functions on port A0 and A1.
//   // This step is not necessary if your part does not support pin muxing.
//   // TODO: change this to select the port/pin you are using.
//   //
//   GPIOPinConfigure(GPIO_PA0_U0RX);
//   GPIOPinConfigure(GPIO_PA1_U0TX);

//   //
//   // Enable UART0 so that we can configure the clock.
//   //
//   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

//   //
//   // Use the internal 16MHz oscillator as the UART clock source.
//   //
//   UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

//   //
//   // Select the alternate (UART) function for these pins.
//   // TODO: change this to select the port/pin you are using.
//   //
//   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

//   //
//   // Initialize the UART for console I/O.
//   //
//   UARTStdioConfig(0, 115200, 16000000);

//   IntMasterEnable(); //enable processor interrupts

//   IntEnable(INT_UART0); //enable the UART interrupt

//   UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
// }

// void UARTIntHandler(void)
// {
//   uint32_t ui32Status;
//   ui32Status = UARTIntStatus(UART0_BASE, 1); //get interrupt status
//   UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
//   while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
//   {
//     CharUART=UARTCharGet(UART0_BASE);
//     if((CharUART>=48 && CharUART<=57)||CharUART == 32 || CharUART == 13)
//     {
//       UARTprintf("%c",CharUART);
//       if(CharUART == 32)
//       { 
//         switch(inc)
//         {
//           case 0:
//             aux_sel = 0;
//           break;
//           case 1: 
//             aux_sel = (CharSel[0]-48);
//           break; 
//           case 2:
//             aux_sel = ((CharSel[1]-48)+(CharSel[0]-48)*10);
//           break;
//           case 3:
//             aux_sel = ((CharSel[0]-48)*100+(CharSel[1]-48)*10+(CharSel[2]-48));
//           break;
//           default:
//             aux_sel = 0;
//           break; 
//         }
//         inc = 0;
//         sel = 1;
//       }
//       else  if(CharUART != 13)
//             {
//               if(!sel)
//               {
//                 if(inc<3)
//                 {
//                   CharSel[inc] = CharUART;
//                   inc++;
//                 }
//                 else
//                 {
//                   for(aux_sel=0;aux_sel<3;aux_sel++) CharSel[aux_sel] = '0';
//                   for(aux_val=0;aux_val<3;aux_val++) CharVal[aux_val] = '0'; 
//                   RefreshUART = 0;
//                   inc = 0;
//                   sel = 0;
//                   UARTprintf("\nCaracteres excedem o limite maximo!");
//                 }
//               }
//               else
//               {
//                 if(inc<3)
//                 {
//                   CharVal[inc] = CharUART;
//                   inc++;
//                 }
//                 else
//                 {
//                   for(aux_sel=0;aux_sel<3;aux_sel++) CharSel[aux_sel] = '0';
//                   for(aux_val=0;aux_val<3;aux_val++) CharVal[aux_val] = '0'; 
//                   RefreshUART = 0;
//                   inc = 0;
//                   sel = 0;
//                   UARTprintf("\nCaracteres excedem o limite maximo!");
//                 }
//               }
//             }
//             else
//             {
//               switch(inc)
//               {
//                 case 0:
//                   aux_val = 0;
//                 break;
//                 case 1: 
//                    aux_val = (CharVal[0]-48);
//                 break; 
//                 case 2:
//                    aux_val = ((CharVal[0]-48)*10+(CharVal[1]-48));
//                 break;
//                 case 3:
//                   aux_val = ((CharVal[0]-48)*100+(CharVal[1]-48)*10+(CharVal[2]-48));
//                 break;
//                 default:
//                   aux_val = 0;
//                 break; 
//               }
//               SendServo(aux_sel, aux_val);
//               inc = 0;
//               sel = 0;
//               for(aux_sel=0;aux_sel<3;aux_sel++) CharSel[aux_sel] = '0';
//               for(aux_val=0;aux_val<3;aux_val++) CharVal[aux_val] = '0'; 
//               RefreshUART = 0;
//               aux_val = 0;
//               aux_sel = aux_val;
//             }
//     }
//     else
//     {
//       UARTprintf("\nCaracter Invalido");
//       RefreshUART = 0;
//     }
//     // //echo character
    
//     // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
//     // delay_ms(20);
//     // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
//     // Switch
//     // SendServo();
//   }
// }

// int main(void)
// { 
//   uint8_t i;

//   CfgClock();
//   CfgModUART();
//   InitServo();

  
//   while(1) 
//   {
//     if(!RefreshUART)
//     {
//       RefreshUART = 1;
//       UARTprintf("\nDigite os valores de saida dos motores:");
//     }
//   }
// }