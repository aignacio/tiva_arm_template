#ifndef MATRIX_8_RGB_C_
#define MATRIX_8_RGB_C_

#include "matrix_8_rgb.h"

volatile uint8_t  LineCounter = 0;

void ClkSSI()
{
  set_high(SCLK_PORT,SCLK_PIN);
  set_low(SCLK_PORT,SCLK_PIN);
}

/*************************************************
  Name:
  bool SendSSITLC(uint8_t matrix, uint16_t offset)

  Function: Write 8x8 RGB Matrix with
  booster pack (TLC5947+HC7438)
  
  Arguments: 
  -matrix - Address of matrix to write
  -offset - Used to animate the matrix

  Return:
  bool - Return 1 if finish to write (Line == 8)
  else return 0;
	
  Obs.: It's necessary to call this 8 times to
  		write all the matrix, when finished
  		return 1.
*************************************************/
bool
SendSSITLC(unsigned char *matrix_t, uint16_t offset)
{ 
  uint8_t led = 0;
  //Will be send 8 RGB led color, one color each 
  //time like R(0)-G(0)-B(0)-R(1)-G(1)-B(1)... 
  //because of this, led < 24
  offset += LineCounter * 24; //We need to forward 24 positions to the next line
  while (led < 24) 
  {
    uint8_t bit_color = 0x80; 
    while (bit_color != 0) 
    {
      //Will be send each bit of 8 bits RGB color
      //one bit for time, until we send a byte
      if(*(matrix_t+led+offset) & bit_color) 
        set_high(MOSI_PORT,MOSI_PIN);
      else  
        set_low(MOSI_PORT,MOSI_PIN);
      ClkSSI();
      bit_color >>= 1;
    }
    set_low(MOSI_PORT,MOSI_PIN);

    uint8_t i;
    for(i=0;i<4;i++)  ClkSSI();
    led++;
  }
  set_high(BLANK_PORT,BLANK_PIN);
  set_custom(LINE_PORT,LINE_PINS,LineCounter<<5);
  set_high(LAT_PORT,LAT_PIN);
  set_low(LAT_PORT,LAT_PIN);  
  set_low(BLANK_PORT,BLANK_PIN);

  LineCounter++;
  if(LineCounter == 8)
  {
    LineCounter = 0;
    return 1;
  }
  else
    return 0;
}


void 
CfgMatrixPins(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_5);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);

  //Comment because we cannot send by SSI2 module
  //Config SSI - 2
  // SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
  // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

  // GPIOPinConfigure(GPIO_PB4_SSI2CLK);
  // GPIOPinConfigure(GPIO_PB5_SSI2FSS);
  // GPIOPinConfigure(GPIO_PB6_SSI2RX);
  // GPIOPinConfigure(GPIO_PB7_SSI2TX);

  // GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |GPIO_PIN_7);
  // //SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_TI, SSI_MODE_MASTER, 1000000, 8);
  // SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_TI, SSI_MODE_MASTER, SysCtlClockGet()/32, 8);
  // SSIEnable(SSI2_BASE);
}


#endif
