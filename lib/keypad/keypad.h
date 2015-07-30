
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
#include <inc/hw_nvic.h>
#include <inc/hw_types.h>

#define COLUMN_0(x) GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,x);
#define COLUMN_1(x) GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,x);
#define COLUMN_2(x) GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_7,x);
#define COLUMN_3(x) GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,x);

uint8_t flag_key = 0,key;

void
IntKeys(void)
{
  GPIOIntClear(GPIO_PORTB_BASE, GPIO_INT_PIN_3);
  GPIOIntClear(GPIO_PORTC_BASE, GPIO_INT_PIN_4 | GPIO_INT_PIN_5 | GPIO_INT_PIN_6);

  uint32_t l;
  COLUMN_0(0xff);
  COLUMN_1(0xff);
  COLUMN_2(0xff);
  COLUMN_3(0xff); 

  for(l = 0; l < 1000; ++l);

  COLUMN_0(0x00);
  COLUMN_1(0xff);
  COLUMN_2(0xff);
  COLUMN_3(0xff);

  if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3))==0){
    key = '1';
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4))==0){
    key = '4'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5))==0){
    key = '7'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6))==0){
    key = '*'; 
  }

  COLUMN_0(0xff);
  COLUMN_1(0x00);
  COLUMN_2(0xff);
  COLUMN_3(0xff);

  if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3))==0){
    key = '2';
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4))==0){
    key = '5';
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5))==0){
    key = '8'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6))==0){
    key = '0'; 
  }

  COLUMN_0(0xff);
  COLUMN_1(0xff);
  COLUMN_2(0x00);
  COLUMN_3(0xff);

  if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3))==0){
    key = '3'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4))==0){
    key = '6'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5))==0){
    key = '9'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6))==0){
    key = '#'; 
  }

  COLUMN_0(0xff);
  COLUMN_1(0xff);
  COLUMN_2(0xff);
  COLUMN_3(0x00);

  if((GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3))==0){
    key = 'A'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4))==0){
    key = 'B'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5))==0){
    key = 'C'; 
  }
  if((GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6))==0){
    key = 'D'; 
  }

  COLUMN_0(0x00);
  COLUMN_1(0x00);
  COLUMN_2(0x00);
  COLUMN_3(0x00);

  flag_key = 1;
  KeyPressedCallback();
}

void 
CfgKeypadInt(void)
{
  //Lines
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

  GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 );

  //Pull-ups
  GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
  GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 ,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
  
  GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_RISING_EDGE);
  GPIOIntTypeSet(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,GPIO_RISING_EDGE);

  GPIOIntRegister(GPIO_PORTB_BASE,IntKeys);
  GPIOIntRegister(GPIO_PORTC_BASE,IntKeys);

  GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_3);
  GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 );

  IntEnable(INT_GPIOB);
  IntEnable(INT_GPIOC);

  IntPrioritySet(INT_GPIOB, 0x00);
  IntPrioritySet(INT_GPIOC, 0x00);

  GPIOIntClear(GPIO_PORTB_BASE, GPIO_INT_PIN_3);
  GPIOIntClear(GPIO_PORTC_BASE, GPIO_INT_PIN_4 | GPIO_INT_PIN_5 | GPIO_INT_PIN_6);

  //Columns
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  GPIOPinUnlockGPIO(GPIO_PORTD_BASE, GPIO_PIN_7);

  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_4);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_6 | GPIO_PIN_7);
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_7);

  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4, ~GPIO_PIN_4);
  GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6, ~GPIO_PIN_6);
  GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_7, ~GPIO_PIN_7);
  GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7, ~GPIO_PIN_7);

  IntMasterEnable();
}
