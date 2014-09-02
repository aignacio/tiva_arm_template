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

/*  Global variables  */
volatile uint32_t CharUART;
volatile uint8_t  CharSel[3]={'000'},CharVal[3]={'000'},inc=0,aux_sel,aux_val;
volatile bool     RefreshUART=0,sel=0;

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
  uint8_t i,variable=0,servo=0,flag_1=0,flag_2=0;

  CfgClock();
  CfgModUART();
  InitServo();
  //lcd_init();
  //lcd_clear();

  //lcd_string("Teste");
  while(1)
  {
	  delay_ms(5);
	  SendServo(servo,variable);
	  if(!flag_1)
		  if(variable < 180) variable++;
		  else flag_1 = 1;
	  else
		  if(variable > 0) variable--;
		  else
			  {
			  	  flag_1 = 0;
			  	  if(servo < 5)	servo++;
			  	  else servo = 0;
			  }


//    if(!RefreshUART)
//    {
//      RefreshUART = 1;
//      UARTprintf("\n[Servo(0-5),Position(0-180)]:");
//    }
  }
}


