/****************************************************************************

	GLCD Library for ARM
	Author: Anderson Ignacio da Silva
	Date: 19/03/2014
	Inf.: http://www.esplatforms.blogspot.com.br

****************************************************************************/
#ifndef MSP_H_
#define MSP_H_

#include "typedefs.h"
//
//
//
void writeData(u_char data);
void writeCommand(u_char command);
void delay(u_char x10ms);
//
#define COMMAND 0
#define DATA 1

#define	glcd_mosi		GPIO_PIN_7			//MOSI PIN=PB6
#define glcd_mosi_port	GPIO_PORTB_BASE
#define	glcd_cs			GPIO_PIN_5			//CS   PIN=PB5
#define glcd_cs_port	GPIO_PORTB_BASE
#define	glcd_sclk		GPIO_PIN_4			//SCLK PIN=PB4
#define glcd_sclk_port	GPIO_PORTB_BASE
#define	glcd_dc			GPIO_PIN_5			//DC   PIN=PE5
#define glcd_dc_port	GPIO_PORTE_BASE

#define LCD_SELECT 		GPIOPinWrite(glcd_cs_port,glcd_cs,~glcd_cs);
#define LCD_DESELECT 	GPIOPinWrite(glcd_cs_port,glcd_cs,glcd_cs);
#define LCD_CLOCK 		GPIOPinWrite(glcd_sclk_port,glcd_sclk,glcd_sclk);	GPIOPinWrite(glcd_sclk_port,glcd_sclk,~glcd_sclk);
#define LCD_SCLK_LO		GPIOPinWrite(glcd_sclk_port,glcd_sclk,~glcd_sclk);
#define LCD_SCLK_HI		GPIOPinWrite(glcd_sclk_port,glcd_sclk,glcd_sclk);
#define LCD_MOSI_HI		GPIOPinWrite(glcd_mosi_port,glcd_mosi,glcd_mosi)
#define LCD_MOSI_LO		GPIOPinWrite(glcd_mosi_port,glcd_mosi,~glcd_mosi)
#define LCD_MOSI_DIR_O  GPIOPinTypeGPIOOutput(glcd_mosi_port, glcd_mosi);
#define LCD_MOSI_DIR_I 	GPIOPinTypeGPIOInput(glcd_mosi_port, glcd_mosi);
#define LCD_DC_HI		GPIOPinWrite(glcd_dc_port,glcd_dc,glcd_dc);
#define LCD_DC_LO		GPIOPinWrite(glcd_dc_port,glcd_dc,~glcd_dc);

#endif /* MSP_H_ */

