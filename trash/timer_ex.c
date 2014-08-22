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
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
//#include <lcd_api.h>

#define PWM_FREQUENCY   50
#define PWM_PERIOD      20000  //Time in us
#define MAX_SERVO_TIME  2400   //Time for max PWM servo (us)
#define MIN_SERVO_TIME  900    //Time for min PWM servo (us)

/*  Global variables  */
volatile uint32_t Counter;
volatile uint8_t flag=1;

/*  Prototypes  */

/*  Functions */
void delay_ms(long delayms)
{
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

void cfg_PWM()
{ 
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //Configure clock to 40MHz

  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);

  TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);

  TimerLoadSet(TIMER0_BASE, TIMER_B, SysCtlClockGet() / 1000000);

  IntMasterEnable();

  TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

  IntEnable(INT_TIMER0B);

  TimerEnable(TIMER0_BASE, TIMER_B);
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


void Timer0BIntHandler(void)
{
  TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

  Counter++;

  if(Counter == 1000000 && flag)
  {
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
    flag = 0;
    Counter = 0;
  }
  else
      if(Counter == 1000000 && !flag)
      {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
        flag = 1;
        Counter = 0;
      }
}

int main(void)
{ 
  cfg_PWM();
  cfg_GPIO();

  while(1);
}
