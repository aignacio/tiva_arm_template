/****************************************************************************

	GLCD Library for ARM
	Author: Anderson Ignacio da Silva
	Date: 19/03/2014
	Inf.: http://www.esplatforms.blogspot.com.br

****************************************************************************/
#include "lcd.h"
#include "glcd_pin.h"
#include "config.h"

u_char _orientation = 0;

u_int getScreenWidth() {
	if (_orientation == 0 || _orientation == 2)
		return SHORT_EDGE_PIXELS;
	else
		return LONG_EDGE_PIXELS;
}

u_int getScreenHeight() {
	if (_orientation == 0 || _orientation == 2)
		return LONG_EDGE_PIXELS;
	else
		return SHORT_EDGE_PIXELS;
}

void setArea(u_int xStart, u_int yStart, u_int xEnd, u_int yEnd) {

	u_char yStartMSB = yStart >> 8;
	u_char yEndMSB = yEnd >> 8;
	u_char xStartMSB = xStart >> 8;
	u_char xEndMSB = xEnd >> 8;

#ifdef ILI9225

#ifdef HORIZONTAL
#define GRXSTART	yStart
#define GRXEND		yEnd
#define GRYSTART	xStart
#define GRYEND		xEnd
#define GRXSTARTMSB	yStartMSB
#define GRXENDMSB		yEndMSB
#define GRYSTARTMSB	xStartMSB
#define GRYENDMSB		xEndMSB
#else
#define GRXSTART	xStart
#define GRXEND		xEnd
#define GRYSTART	yStart
#define GRYEND		yEnd
#define GRXSTARTMSB	xStartMSB
#define GRXENDMSB		xEndMSB
#define GRYSTARTMSB	yStartMSB
#define GRYENDMSB		yEndMSB
#endif

	writeCommand(ILIGRAMHEA);
	writeData(GRXENDMSB);
	writeData(GRXEND);
	writeCommand(ILIGRAMHSA);
	writeData(GRXSTARTMSB);
	writeData(GRXSTART);

	writeCommand(ILIGRAMVEA);
	writeData(GRYENDMSB);
	writeData(GRYEND);
	writeCommand(ILIGRAMVSA);
	writeData(GRYSTARTMSB);
	writeData(GRYSTART);

	writeCommand(ILIGRAMADDRX);
	writeData(GRXSTARTMSB);
	writeData(GRXSTART);
	writeCommand(ILIGRAMADDRY);
	writeData(GRYSTARTMSB);
	writeData(GRYSTART);

	writeCommand(ILIWRDATA);

#else
	writeCommand(CASETP);

	writeData(xStartMSB);
	writeData(xStart);

	writeData(xEndMSB);
	writeData(xEnd);

	writeCommand(PASETP);

	writeData(yStartMSB);
	writeData(yStart);

	writeData(yEndMSB);
	writeData(yEnd);

	writeCommand(RAMWRP);
#endif //ILI switch
	// data to follow
}

////////////////////////////////////
// gamma, lut, and other inits
////////////////////////////////////

/////////////////////////////////////////////////
// ILI9340 based display
/////////////////////////////////////////////////
#ifdef ILI9340

void initLCD() {
	writeCommand(0xCB);
	writeData(0x39);
	writeData(0x2C);
	writeData(0x00);
	writeData(0x34);
	writeData(0x02);

	writeCommand(0xCF);
	writeData(0x00);
	writeData(0XC1);
	writeData(0X30);

	writeCommand(0xE8);
	writeData(0x85);
	writeData(0x00);
	writeData(0x78);

	writeCommand(0xEA);
	writeData(0x00);
	writeData(0x00);

	writeCommand(0xED);
	writeData(0x64);
	writeData(0x03);
	writeData(0X12);
	writeData(0X81);

	writeCommand(0xF7);
	writeData(0x20);

	writeCommand(ILIPC1);
	writeData(0x23);
	writeCommand(ILIPC2);
	writeData(0x10);
	writeCommand(ILIVC1);
	writeData(0x3e);
	writeData(0x28);
	writeCommand(ILIVC2);
	writeData(0x86);

	setOrientation(ORIENTATION);

	writeCommand(ILIPFS);
	writeData(0x55);

	writeCommand(ILIFCNM);
	writeData(0x00);
	writeData(0x18);

	writeCommand(ILIDFC);
	writeData(0x08);
	writeData(0x82);
	writeData(0x27);

	writeCommand(ILIGFD);
	writeData(0x00);
	writeCommand(ILIGS);
	writeData(0x01);

	writeCommand(ILIPGC);
	const u_char gamma1[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
			0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
	u_char c = 0;
	while (c < 16) {
		writeData(gamma1[c]);
		c++;
	}

	writeCommand(ILINGC);
	const u_char gamma2[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
			0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
	c = 0;
	while (c < 16) {
		writeData(gamma2[c]);
		c++;
	}

	writeCommand(SLEEPOUT);
	delay(12);
	writeCommand(DISPON);
	writeCommand(RAMWRP);

}

void setOrientation(u_char orientation) {
	writeCommand(ILIMAC);

	switch (orientation) {
	case 1:
		writeData(0xE8);
		_orientation = 1;
		break;
	case 2:
		writeData(0x88);
		_orientation = 2;
		break;
	case 3:
		writeData(0x28);
		_orientation = 3;
		break;
	default:
		writeData(0x48);
		_orientation = 0;
	}
}

#endif
/////////////////////////////////////////////////
// ST7735 based display
/////////////////////////////////////////////////
#ifdef ST7735

void gammaAdjustmentST7735() {
	const u_char gmctrp1[] = {0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d,
		0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
	const u_char gmctrn1[] = {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
		0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
// gamma correction is needed for accurate color but is not necessary.
	u_char c = 0;
	writeCommand(ST7735_GMCTRP1);// gamma adjustment (+ polarity)
	while (c < 16) {
		writeData(gmctrp1[c]);
		c++;
	}
	c = 0;
	writeCommand(ST7735_GMCTRN1); // gamma adjustment (- polarity)
	while (c < 16) {
		writeData(gmctrn1[c]);
		c++;
	}
}

void initLCD() {

	writeCommand(SWRESET);
	delay(20);
	writeCommand(SLEEPOUT);
	delay(20); // driver is doing self check, but seems to be working fine without the delay
	writeCommand(COLMOD);
	writeData(0x05);// 16-bit mode
	writeCommand(MADCTL);
#ifdef HORIZONTAL
	writeData(0x68); //horiz
#else
	writeData(0xC8); // our display is rotated
#endif
	//writeCommand(SETCON);
	//writeData(0x3F);
	gammaAdjustmentST7735();

	writeCommand(DISPON);// why? DISPON should be set on power up, sleep out should be enough
}

#endif

/////////////////////////////////////////////////
// HX8340 based display
/////////////////////////////////////////////////
#ifdef HX8340

void gammaAdjustmentHX8340() {
	const u_char gammap[] = {0x60, 0x71, 0x01, 0x0E, 0x05, 0x02, 0x09, 0x31,
		0x0A}; //9th is CGM
	const u_char gamman[] = {0x67, 0x30, 0x61, 0x17, 0x48, 0x07, 0x05, 0x33};
	u_char c = 0;
	writeCommand(SETGAMMAP);
	while (c < 9) {
		writeData(gammap[c]);
		c++;
	}
	c = 0;
	writeCommand(SETGAMMAN);
	while (c < 8) {
		writeData(gamman[c]);
		c++;
	}
}

void initLCD() {
	delay(20);
	writeCommand(SETEXTCMD);
	writeData(0xFF);
	writeData(0x83);
	writeData(0x40);
	writeCommand(SLEEPOUT);
	delay(15);
	writeCommand(0xCA); //?
	writeData(0x70);
	writeData(0x00);
	writeData(0xD9);
	writeCommand(SETOSC);
	writeData(0x01);
	writeData(0x11);
	writeCommand(0xC9);
	writeData(0x90);
	writeData(0x49);
	writeData(0x10);
	writeData(0x28);
	writeData(0x28);
	writeData(0x10);
	writeData(0x00);
	writeData(0x06);
	delay(2);
// gamma
	gammaAdjustmentHX8340();
//
	delay(1);
	writeCommand(SETPWCTR5);
	writeData(0x35);
	writeData(0x20);
	writeData(0x45);
	writeCommand(SETPWCTR4);
	writeData(0x33);
	writeData(0x25);
	writeData(0x4C);
	delay(1);
	writeCommand(COLMOD);
	writeData(0x05);
	// orientation
	writeCommand(0x36);
#ifdef HORIZONTAL
	writeData(0xA8); //horiz
#else
	writeData(0x08);
#endif
	//
	writeCommand(DISPON);

}

#endif

/////////////////////////////////////////////////
// ILI9225 based display
/////////////////////////////////////////////////
#ifdef ILI9225

void gammaAdjustmentILI9225() {
	const u_char dataMSB[] = {0x00, 0x09, 0x0d, 0x09, 0x04, 0x05, 0x00, 0x00,
		0x17, 0x00};
	const u_char dataLSB[] = {0x03, 0x00, 0x05, 0x00, 0x07, 0x02, 0x00, 0x05,
		0x00, 0x1F};
	const u_char cmd = 0x50;
	u_char c = 0;
	while (c < 10) {
		writeCommand(cmd + c);
		writeData(dataMSB[c]);
		writeData(dataLSB[c]);
		c++;
	}
}

void setGRAMILI9225() {
	const u_char dataLSB[] = {0x00, 0xDB, 0x00, 0x00, 0xDB, 0x00, 0xAF, 0x00,
		0xDB, 0x00};
	const u_char cmd = 0x30;
	u_char c = 0;
	while (c < 10) {
		writeCommand(cmd + c);
		writeData(0x00);
		writeData(dataLSB[c]);
		c++;
	}
}

void initLCD() {

	writeCommand(0x28);
	delay(20);
	writeCommand(0x01);
#ifdef HORIZONTAL
	writeData(0x03);
#else
	writeData(0x01);
#endif
	writeData(0x1C);
	writeCommand(0x02);
	writeData(0x01); // set 1 line inversion
	writeData(0x00);
	writeCommand(0x03);
	writeData(0x10);// set GRAM write direction and BGR=1.//1030
#ifdef HORIZONTAL
	writeData(0x38);
#else
	writeData(0x30);
#endif
	writeCommand(0x08);
	writeData(0x08); // set BP and FP
	writeData(0x08);
	writeCommand(0x0C);
	writeData(0x00);// RGB interface setting 0x0110 for RGB 18Bit and 0111for RGB16Bit
	writeData(0x00);
	writeCommand(0x0F);
	writeData(0x0b);// Set frame rate//0b01
	writeData(0x01);
	delay(5);
	writeCommand(0x10);
	writeData(0x0a);// Set SAP,DSTB,STB//0800
	writeData(0x00);
	writeCommand(0x11);
	writeData(0x10);// Set APON,PON,AON,VCI1EN,VC
	writeData(0x38);
	delay(5);
	writeCommand(0x12);
	writeData(0x11);// Internal reference voltage= Vci;
	writeData(0x21);
	writeCommand(0x13);
	writeData(0x00);// Set GVDD
	writeData(0x63);
	writeCommand(0x14);
	writeData(0x4b);// Set VCOMH/VCOML voltage//3944
	writeData(0x44);
	writeCommand(ILIGRAMADDRX);
	writeData(0x00);
	writeData(0x00);
	writeCommand(ILIGRAMADDRY);
	writeData(0x00);
	writeData(0x00);
	setGRAMILI9225();
	gammaAdjustmentILI9225();

	delay(5);
	writeCommand(0x07);
	writeData(0x10);
	writeData(0x17);

}

#endif

