/****************************************************************************

	GLCD Library for ARM
	Author: Anderson Ignacio da Silva
	Date: 19/03/2014
	Inf.: http://www.esplatforms.blogspot.com.br

****************************************************************************/
#define	PART_TM4C123GH6PM
#define	TARGET_IS_BLIZZARD_RB1

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"

#include "glcd_pin.h"
#include "lcd.h"
#include "graphics.h"
#include "color.h"
#include "typedefs.h"


u_char cc = 0;
u_char sx = 0;
u_char sy = 0;
u_char d = 0;
u_char o = 0;
u_char orientation = 0;

const u_char starty[13] = { 30, 48, 150, 50, 40, 85, 38, 105, 66, 130, 35, 71, 117 };
const u_char startx[17] = { 180, 30, 100, 96, 48, 50, 150, 70, 40, 38, 123, 132,
		80, 35, 110, 55, 99 };
const u_char dim[9] = { 20, 14, 5, 17, 10, 5, 3, 18, 12 };
const u_char offset[10] = { 6, 4, 1, 9, 7, 2, 8, 0, 3, 5 };
void drawLogicLines(u_char h);
void shesGotColors(u_int repeatTimes);

void setUpMCU();

void main(void) {

		SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

		initLCD();


	initLCD();

	while (1) {
		delay(10);
		clearScreen(1);

		setColor(COLOR_16_RED);
		drawString(5, 5, FONT_SM, "Texas Instruments");
		setColor(COLOR_16_WHITE);
		drawString(5, 20, FONT_MD, "LaunchPad Booster Pack");
		setColor(COLOR_16_BLUE);
		drawString(5, 40, FONT_LG, "2.2\" Color LCD");
		setColor(COLOR_16_YELLOW);
		drawString(5, 60, FONT_LG, "MSP430, Stellaris, Tiva");
		setColor(COLOR_16_ORANGE);
		drawString(5, 80, FONT_MD, "240 x 320 pixels");
		setColor(COLOR_16_PURPLE);
		drawString(5, 100, FONT_MD, "16bit color");

		delay(100);
		//
		shesGotColors(50);
		//
		delay(100);
		//
		clearScreen(1);
		//
		drawLogicLines(8);
		delay(100);

		setOrientation(++orientation & 0x03);
	}
}

///////////////////////////
// needed for test only
///////////////////////////
void drawLogicLines(u_char h) {
	u_char * addr = 0;
	u_int y = 0;
	u_char c = 1;
	while (y < getScreenHeight()) {
		setColor(colors[c]);
		drawLogicLine(1, y, getScreenWidth(), h, (u_char*) addr);
		y += (h + 2);
		c++;
		addr += getScreenWidth();
	}
}

u_int getColor() {
	cc++;
	if (cc == 43)
		cc = 0;
	return colors[cc];
}

u_char getX() {
	sx++;
	if (sx == 17)
		sx = 0;
	return startx[sx];
}

u_char getY() {
	sy++;
	if (sy == 13)
		sy = 0;
	return starty[sy];
}

u_char getD() {
	d++;
	if (d == 9)
		d = 0;
	return dim[d];
}

u_char getO() {
	o++;
	if (o == 10)
		o = 0;
	return offset[o];
}

void shesGotColors(u_int times) {

	u_char x;
	u_char y;
	u_char d;
	u_int repeatTimes = times;

	while (repeatTimes > 0) {
		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawLine(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		fillRect(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawRect(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		fillCircle(x, y, d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawLine(x - d - getO(), y + d + getO(), x + d + getO(),
				y - d - getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawCircle(x, y, d + getO());

		repeatTimes--;
	}
}
/////////////////////////////////////////
// end of test
/////////////////////////////////////////




