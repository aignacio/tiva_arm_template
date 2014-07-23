/****************************************************************************

	GLCD Library for ARM
	Author: Anderson Ignacio da Silva
	Date: 19/03/2014
	Inf.: http://www.esplatforms.blogspot.com.br

****************************************************************************/
#include "glcd_pin.h"
#include "lcd.h"
#include "config.h"
#include "driverlib/gpio.c"


////////////////////////////////////
// write & delay
////////////////////////////////////

// code duplication, but we are saving clock cycles by not passing dataCommand
void writeData(u_char data) {
	LCD_SELECT;
	LCD_DC_HI;

	u_char c = 0;
	while (c < 8) {
		(data & 0x80) ? (LCD_MOSI_HI) : (LCD_MOSI_LO);
		LCD_CLOCK
		;
		data <<= 1;
		c++;
	}

	LCD_DESELECT;
}

// code duplication, but we are saving clock cycles by not passing dataCommand
void writeCommand(u_char command) {
	LCD_SELECT;
	LCD_DC_LO;

	u_char c = 0;
	while (c < 8) {
		(command & 0x80) ? (LCD_MOSI_HI) : (LCD_MOSI_LO);
		LCD_CLOCK
		;
		command <<= 1;
		c++;
	}

	LCD_DESELECT;
}

/*
 * long delay
 */
void delay(u_char x10ms) {
	while (x10ms > 0) {
		int c = 10000;
		while(c != 0) c--;
		x10ms--;
	}
}

