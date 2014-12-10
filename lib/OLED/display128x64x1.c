//*****************************************************************************
//
// display128x64x1.c - Driver for the 128x64 monochrome graphical OLED
//                    displays based on SSD1306 controller
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE AUTHOR SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// AUTHOR: JOERG QUINTEN
// E2E-NICKNAME: aBUGSworstnightmare
//
// Revision: r03
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "I2C_Stellaris_API.h"
#include "display128x64x1.h"
#include "logos.h"

//*****************************************************************************
//
// A 5x7 font (in a 6x8 cell) for displaying text on the OLED display.
// The data is organized as bytes from the left column to the right column,
// with each byte containing the top row in the LSB and the bottom row in the MSB.
//
//*****************************************************************************
static const char g_pucFont[122][6] =
{
		{ 0x00,0x00,0x00,0x00,0x00,0x00},
		{ 0x00,0x00,0x4F,0x00,0x00,0x00}, //   (  1)  ! - 0x0021 Exclamation Mark
		{ 0x00,0x07,0x00,0x07,0x00,0x00}, //   (  2)  " - 0x0022 Quotation Mark
		{ 0x14,0x7F,0x14,0x7F,0x14,0x00}, //   (  3)  # - 0x0023 Number Sign
		{ 0x24,0x2A,0x7F,0x2A,0x12,0x00}, //   (  4)  $ - 0x0024 Dollar Sign
		{ 0x23,0x13,0x08,0x64,0x62,0x00}, //   (  5)  % - 0x0025 Percent Sign
		{ 0x36,0x49,0x55,0x22,0x50,0x00}, //   (  6)  & - 0x0026 Ampersand
		{ 0x00,0x05,0x03,0x00,0x00,0x00}, //   (  7)  ' - 0x0027 Apostrophe
		{ 0x00,0x1C,0x22,0x41,0x00,0x00}, //   (  8)  ( - 0x0028 Left Parenthesis
		{ 0x00,0x41,0x22,0x1C,0x00,0x00}, //   (  9)  ) - 0x0029 Right Parenthesis
		{ 0x14,0x08,0x3E,0x08,0x14,0x00}, //   ( 10)  * - 0x002A Asterisk
		{ 0x08,0x08,0x3E,0x08,0x08,0x00}, //   ( 11)  + - 0x002B Plus Sign
		{ 0x00,0x50,0x30,0x00,0x00,0x00}, //   ( 12)  , - 0x002C Comma
		{ 0x08,0x08,0x08,0x08,0x08,0x00}, //   ( 13)  - - 0x002D Hyphen-Minus
		{ 0x00,0x60,0x60,0x00,0x00,0x00}, //   ( 14)  . - 0x002E Full Stop
		{ 0x20,0x10,0x08,0x04,0x02,0x00}, //   ( 15)  / - 0x002F Solidus
		{ 0x3E,0x51,0x49,0x45,0x3E,0x00}, //   ( 16)  0 - 0x0030 Digit Zero
		{ 0x00,0x42,0x7F,0x40,0x00,0x00}, //   ( 17)  1 - 0x0031 Digit One
		{ 0x42,0x61,0x51,0x49,0x46,0x00}, //   ( 18)  2 - 0x0032 Digit Two
		{ 0x21,0x41,0x45,0x4B,0x31,0x00}, //   ( 19)  3 - 0x0033 Digit Three
		{ 0x18,0x14,0x12,0x7F,0x10,0x00}, //   ( 20)  4 - 0x0034 Digit Four
		{ 0x27,0x45,0x45,0x45,0x39,0x00}, //   ( 21)  5 - 0x0035 Digit Five
		{ 0x3C,0x4A,0x49,0x49,0x30,0x00}, //   ( 22)  6 - 0x0036 Digit Six
		{ 0x01,0x71,0x09,0x05,0x03,0x00}, //   ( 23)  7 - 0x0037 Digit Seven
		{ 0x36,0x49,0x49,0x49,0x36,0x00}, //   ( 24)  8 - 0x0038 Digit Eight
		{ 0x06,0x49,0x49,0x29,0x1E,0x00}, //   ( 25)  9 - 0x0039 Dight Nine
		{ 0x00,0x36,0x36,0x00,0x00,0x00}, //   ( 26)  : - 0x003A Colon
		{ 0x00,0x56,0x36,0x00,0x00,0x00}, //   ( 27)  ; - 0x003B Semicolon
		{ 0x08,0x14,0x22,0x41,0x00,0x00}, //   ( 28)  < - 0x003C Less-Than Sign
		{ 0x14,0x14,0x14,0x14,0x14,0x00}, //   ( 29)  = - 0x003D Equals Sign
		{ 0x00,0x41,0x22,0x14,0x08,0x00}, //   ( 30)  > - 0x003E Greater-Than Sign
		{ 0x02,0x01,0x51,0x09,0x06,0x00}, //   ( 31)  ? - 0x003F Question Mark
		{ 0x32,0x49,0x79,0x41,0x3E,0x00}, //   ( 32)  @ - 0x0040 Commercial At
		{ 0x7E,0x11,0x11,0x11,0x7E,0x00}, //   ( 33)  A - 0x0041 Latin Capital Letter A
		{ 0x7F,0x49,0x49,0x49,0x36,0x00}, //   ( 34)  B - 0x0042 Latin Capital Letter B
		{ 0x3E,0x41,0x41,0x41,0x22,0x00}, //   ( 35)  C - 0x0043 Latin Capital Letter C
		{ 0x7F,0x41,0x41,0x22,0x1C,0x00}, //   ( 36)  D - 0x0044 Latin Capital Letter D
		{ 0x7F,0x49,0x49,0x49,0x41,0x00}, //   ( 37)  E - 0x0045 Latin Capital Letter E
		{ 0x7F,0x09,0x09,0x09,0x01,0x00}, //   ( 38)  F - 0x0046 Latin Capital Letter F
		{ 0x3E,0x41,0x49,0x49,0x7A,0x00}, //   ( 39)  G - 0x0047 Latin Capital Letter G
		{ 0x7F,0x08,0x08,0x08,0x7F,0x00}, //   ( 40)  H - 0x0048 Latin Capital Letter H
		{ 0x00,0x41,0x7F,0x41,0x00,0x00}, //   ( 41)  I - 0x0049 Latin Capital Letter I
		{ 0x20,0x40,0x41,0x3F,0x01,0x00}, //   ( 42)  J - 0x004A Latin Capital Letter J
		{ 0x7F,0x08,0x14,0x22,0x41,0x00}, //   ( 43)  K - 0x004B Latin Capital Letter K
		{ 0x7F,0x40,0x40,0x40,0x40,0x00}, //   ( 44)  L - 0x004C Latin Capital Letter L
		{ 0x7F,0x02,0x0C,0x02,0x7F,0x00}, //   ( 45)  M - 0x004D Latin Capital Letter M
		{ 0x7F,0x04,0x08,0x10,0x7F,0x00}, //   ( 46)  N - 0x004E Latin Capital Letter N
		{ 0x3E,0x41,0x41,0x41,0x3E,0x00}, //   ( 47)  O - 0x004F Latin Capital Letter O
		{ 0x7F,0x09,0x09,0x09,0x06,0x00}, //   ( 48)  P - 0x0050 Latin Capital Letter P
		{ 0x3E,0x41,0x51,0x21,0x5E,0x00}, //   ( 49)  Q - 0x0051 Latin Capital Letter Q
		{ 0x7F,0x09,0x19,0x29,0x46,0x00}, //   ( 50)  R - 0x0052 Latin Capital Letter R
		{ 0x46,0x49,0x49,0x49,0x31,0x00}, //   ( 51)  S - 0x0053 Latin Capital Letter S
		{ 0x01,0x01,0x7F,0x01,0x01,0x00}, //   ( 52)  T - 0x0054 Latin Capital Letter T
		{ 0x3F,0x40,0x40,0x40,0x3F,0x00}, //   ( 53)  U - 0x0055 Latin Capital Letter U
		{ 0x1F,0x20,0x40,0x20,0x1F,0x00}, //   ( 54)  V - 0x0056 Latin Capital Letter V
		{ 0x3F,0x40,0x38,0x40,0x3F,0x00}, //   ( 55)  W - 0x0057 Latin Capital Letter W
		{ 0x63,0x14,0x08,0x14,0x63,0x00}, //   ( 56)  X - 0x0058 Latin Capital Letter X
		{ 0x07,0x08,0x70,0x08,0x07,0x00}, //   ( 57)  Y - 0x0059 Latin Capital Letter Y
		{ 0x61,0x51,0x49,0x45,0x43,0x00}, //   ( 58)  Z - 0x005A Latin Capital Letter Z
		{ 0x00,0x7F,0x41,0x41,0x00,0x00}, //   ( 59)  [ - 0x005B Left Square Bracket
		{ 0x02,0x04,0x08,0x10,0x20,0x00}, //   ( 60)  \ - 0x005C Reverse Solidus
		{ 0x00,0x41,0x41,0x7F,0x00,0x00}, //   ( 61)  ] - 0x005D Right Square Bracket
		{ 0x04,0x02,0x01,0x02,0x04,0x00}, //   ( 62)  ^ - 0x005E Circumflex Accent
		{ 0x40,0x40,0x40,0x40,0x40,0x00}, //   ( 63)  _ - 0x005F Low Line
		{ 0x01,0x02,0x04,0x00,0x00,0x00}, //   ( 64)  ` - 0x0060 Grave Accent
		{ 0x20,0x54,0x54,0x54,0x78,0x00}, //   ( 65)  a - 0x0061 Latin Small Letter A
		{ 0x7F,0x48,0x44,0x44,0x38,0x00}, //   ( 66)  b - 0x0062 Latin Small Letter B
		{ 0x38,0x44,0x44,0x44,0x20,0x00}, //   ( 67)  c - 0x0063 Latin Small Letter C
		{ 0x38,0x44,0x44,0x48,0x7F,0x00}, //   ( 68)  d - 0x0064 Latin Small Letter D
		{ 0x38,0x54,0x54,0x54,0x18,0x00}, //   ( 69)  e - 0x0065 Latin Small Letter E
		{ 0x08,0x7E,0x09,0x01,0x02,0x00}, //   ( 70)  f - 0x0066 Latin Small Letter F
		{ 0x06,0x49,0x49,0x49,0x3F,0x00}, //   ( 71)  g - 0x0067 Latin Small Letter G
		{ 0x7F,0x08,0x04,0x04,0x78,0x00}, //   ( 72)  h - 0x0068 Latin Small Letter H
		{ 0x00,0x44,0x7D,0x40,0x00,0x00}, //   ( 73)  i - 0x0069 Latin Small Letter I
		{ 0x20,0x40,0x44,0x3D,0x00,0x00}, //   ( 74)  j - 0x006A Latin Small Letter J
		{ 0x7F,0x10,0x28,0x44,0x00,0x00}, //   ( 75)  k - 0x006B Latin Small Letter K
		{ 0x00,0x41,0x7F,0x40,0x00,0x00}, //   ( 76)  l - 0x006C Latin Small Letter L
		{ 0x7C,0x04,0x18,0x04,0x7C,0x00}, //   ( 77)  m - 0x006D Latin Small Letter M
		{ 0x7C,0x08,0x04,0x04,0x78,0x00}, //   ( 78)  n - 0x006E Latin Small Letter N
		{ 0x38,0x44,0x44,0x44,0x38,0x00}, //   ( 79)  o - 0x006F Latin Small Letter O
		{ 0x7C,0x14,0x14,0x14,0x08,0x00}, //   ( 80)  p - 0x0070 Latin Small Letter P
		{ 0x08,0x14,0x14,0x18,0x7C,0x00}, //   ( 81)  q - 0x0071 Latin Small Letter Q
		{ 0x7C,0x08,0x04,0x04,0x08,0x00}, //   ( 82)  r - 0x0072 Latin Small Letter R
		{ 0x48,0x54,0x54,0x54,0x20,0x00}, //   ( 83)  s - 0x0073 Latin Small Letter S
		{ 0x04,0x3F,0x44,0x40,0x20,0x00}, //   ( 84)  t - 0x0074 Latin Small Letter T
		{ 0x3C,0x40,0x40,0x20,0x7C,0x00}, //   ( 85)  u - 0x0075 Latin Small Letter U
		{ 0x1C,0x20,0x40,0x20,0x1C,0x00}, //   ( 86)  v - 0x0076 Latin Small Letter V
		{ 0x3C,0x40,0x30,0x40,0x3C,0x00}, //   ( 87)  w - 0x0077 Latin Small Letter W
		{ 0x44,0x28,0x10,0x28,0x44,0x00}, //   ( 88)  x - 0x0078 Latin Small Letter X
		{ 0x0C,0x50,0x50,0x50,0x3C,0x00}, //   ( 89)  y - 0x0079 Latin Small Letter Y
		{ 0x44,0x64,0x54,0x4C,0x44,0x00}, //   ( 90)  z - 0x007A Latin Small Letter Z
		{ 0x00,0x08,0x36,0x41,0x00,0x00}, //   ( 91)  { - 0x007B Left Curly Bracket
		{ 0x00,0x00,0x7F,0x00,0x00,0x00}, //   ( 92)  | - 0x007C Vertical Line
		{ 0x00,0x41,0x36,0x08,0x00,0x00}, //   ( 93)  } - 0x007D Right Curly Bracket
		{ 0x02,0x01,0x02,0x04,0x02,0x00}, //   ( 94)  ~ - 0x007E Tilde
		{ 0x3E,0x55,0x55,0x41,0x22,0x00}, //   ( 95)  C - 0x0080 <Control>
		{ 0x00,0x00,0x00,0x00,0x00,0x00}, //   ( 96)    - 0x00A0 No-Break Space
		{ 0x00,0x00,0x79,0x00,0x00,0x00}, //   ( 97)  ! - 0x00A1 Inverted Exclamation Mark
		{ 0x18,0x24,0x74,0x2E,0x24,0x00}, //   ( 98)  c - 0x00A2 Cent Sign
		{ 0x48,0x7E,0x49,0x42,0x40,0x00}, //   ( 99)  L - 0x00A3 Pound Sign
		{ 0x5D,0x22,0x22,0x22,0x5D,0x00}, //   (100)  o - 0x00A4 Currency Sign
		{ 0x15,0x16,0x7C,0x16,0x15,0x00}, //   (101)  Y - 0x00A5 Yen Sign
		{ 0x00,0x00,0x77,0x00,0x00,0x00}, //   (102)  | - 0x00A6 Broken Bar
		{ 0x0A,0x55,0x55,0x55,0x28,0x00}, //   (103)    - 0x00A7 Section Sign
		{ 0x00,0x01,0x00,0x01,0x00,0x00}, //   (104)  " - 0x00A8 Diaeresis
		{ 0x00,0x0A,0x0D,0x0A,0x04,0x00}, //   (105)    - 0x00AA Feminine Ordinal Indicator
		{ 0x08,0x14,0x2A,0x14,0x22,0x00}, //   (106) << - 0x00AB Left-Pointing Double Angle Quotation Mark
		{ 0x04,0x04,0x04,0x04,0x1C,0x00}, //   (107)    - 0x00AC Not Sign
		{ 0x00,0x08,0x08,0x08,0x00,0x00}, //   (108)  - - 0x00AD Soft Hyphen
		{ 0x01,0x01,0x01,0x01,0x01,0x00}, //   (109)    - 0x00AF Macron
		{ 0x00,0x02,0x05,0x02,0x00,0x00}, //   (110)    - 0x00B0 Degree Sign
		{ 0x44,0x44,0x5F,0x44,0x44,0x00}, //   (111) +- - 0x00B1 Plus-Minus Sign
		{ 0x00,0x00,0x04,0x02,0x01,0x00}, //   (112)  ` - 0x00B4 Acute Accent
		{ 0x7E,0x20,0x20,0x10,0x3E,0x00}, //   (113)  u - 0x00B5 Micro Sign
		{ 0x06,0x0F,0x7F,0x00,0x7F,0x00}, //   (114)    - 0x00B6 Pilcrow Sign
		{ 0x00,0x18,0x18,0x00,0x00,0x00}, //   (115)  . - 0x00B7 Middle Dot
		{ 0x00,0x40,0x50,0x20,0x00,0x00}, //   (116)    - 0x00B8 Cedilla
		{ 0x00,0x0A,0x0D,0x0A,0x00,0x00}, //   (117)    - 0x00BA Masculine Ordinal Indicator
		{ 0x22,0x14,0x2A,0x14,0x08,0x00}, //   (118) >> - 0x00BB Right-Pointing Double Angle Quotation Mark
		{ 0x17,0x08,0x34,0x2A,0x7D,0x00}, //   (119) /4 - 0x00BC Vulgar Fraction One Quarter
		{ 0x17,0x08,0x04,0x6A,0x59,0x00}, //   (120) /2 - 0x00BD Vulgar Fraction One Half
		{ 0x30,0x48,0x45,0x40,0x20,0x00}, //   (121)  ? - 0x00BF Inverted Question Mark
};

//*****************************************************************************
//
// The sequence of commands used to initialize the SSD1306 controller
//
//*****************************************************************************
static const char g_pucSSDInit[] =
{
		// First 0x80 command byte is in function call
		// Display off
		SSD1308_DISPLAYOFF,

		// SET All pixels OFF
		CMDBYTE,SSD1308_DISPLAYALLONRESUME,

		// Set Memory Addressing Mode (command = 1.byte)
		CMDBYTE,SSD1308_MEMORYMODE,
		// to Page addressing mode (mode = 2.byte)
		CMDBYTE,SSD1308_PAGEMODE,

		// Set Lower Column Start Address for Page Addressing Mode
		CMDBYTE,SSD1308_SETLOWCOMPAGEMODE,

		// Set Higher Column Start Address for Page Addressing Mode
		CMDBYTE,SSD1308_SETHIGHCOMPAGEMODE,

		// Set Display Start Line
		CMDBYTE,SSD1308_DISPLAYSTARTLINE_RST,

		// Set Display Double Size Font (DSIZE, command = 1.byte)
		CMDBYTE,SSD1308_DISPLAY_FONT_DSIZE,
		// OFF (2.byte)
		CMDBYTE,SSD1308_DISPLAY_FONT_DSIZE_OFF,

		// Set Display Contrast Control (command = 1.byte)
		CMDBYTE,SSD1308_DISPLAYCONTRAST,
		// contrast for Wide.hk o.96in OLED (contrast value = 2.byte)
		CMDBYTE,SSD308_DISPLAYCONTRASTWHK,

		// Set Segment Re-Map
		CMDBYTE,SSD1308_DISPLAYMAP,

		// Entire Display ON
		// Output follows RAM content
		CMDBYTE,SSD1308_DISPLAYALLONRESUME,

		// Set Normal/Inverse Display
		// white chars on black background
		CMDBYTE,SSD1308_DISPLAYNORMAL,

		// Set Multiplex Ratio (command = 1.byte)
		CMDBYTE,SSD1308_SETMULTIPLEX,
		// 1/64 Multiplex (RESET) (multiplex ratio = 2.byte)
		CMDBYTE,SSD1308_MULTIRATIO,

		// External or internal IREF Selection (command = 1.byte)
		CMDBYTE,SSD1308_DISPLAYIREF,
		// External IREF (RESET)
		CMDBYTE,SSD1308_DISPLAYIREFEXT,

		// Set Page Start Address for Page Addressing Mode
		CMDBYTE,SSD1308_STARTPAGE_0,

		// Set COM Output Scan Direction
		CMDBYTE,SSD1308_COMDIRECTIONMAP,

		// Set Display Offset (command = 1.byte)
		CMDBYTE,SSD1308_DISPLAYOFFSET,
		// no offset (RESET) (= 2.byte)
		CMDBYTE,SSD1308_DISPLAY_NOOFFSET,

		// Display ON
		CMDBYTE,SSD1308_DISPLAYON
};

//*****************************************************************************
//
// The sequence of commands used to set the cursor to the first column of a page
//
//*****************************************************************************
static const char g_pucSSDRowStart[] =
{
		// Set Display Start Line
		// first 0x80 command byte is in function call
		SSD1308_DISPLAYSTARTLINE_RST,
		// Set Lower Column Start Address for Page Addressing Mode
		CMDBYTE,SSD1308_SETLOWCOMPAGEMODE,
		// Set Higher Column Start Address for Page Addressing Mode
		CMDBYTE,SSD1308_SETHIGHCOMPAGEMODE
};


//*****************************************************************************
//
// The sequence of commands used to shut the display off.
//
//*****************************************************************************
static const char g_displayoff[] =
{
		// first 0x80 command byte is in function call
		SSD1308_DISPLAYOFF
};

//*****************************************************************************
//
//! Turns on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//!
//! This function will turn on the OLED display, causing it to display the
//! contents of its internal frame buffer.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1DisplayOn(unsigned long ulI2CBase)
{
    //
    // Re-/Initialize the display controller.
    //
	I2CWriteData(ulI2CBase, SSD_ADDR , CMDBYTE, g_pucSSDInit, sizeof(g_pucSSDInit));
}

//*****************************************************************************
//
//! Turns off the OLED display.
//!
//! This function will turn off the OLED display.  This will stop the scanning
//! of the panel and turn off the on-chip DC-DC converter (if prsent),
//! preventing damage to the panel due to burn-in (it has similar characters
//! to a CRT in this respect).
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1DisplayOff(unsigned long ulI2CBase)
{
    //
    // Turn off the DC-DC converter (if any) and the display.
    //
	I2CWriteData(ulI2CBase, SSD_ADDR , CMDBYTE, g_displayoff, sizeof(g_displayoff));
}


//*****************************************************************************
//
//! Clears a single line on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param ulY is the display line to be cleared, 0 to 7.
//!
//! This function will clear one text line of the display.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1ClearLine(unsigned long ulI2CBase, unsigned long ulY)
{
	char pcBuf[DISPLAY_WIDTH];
	unsigned long ulIdx;

    //
    // Fill the buffer with zeros
    //
    for(ulIdx = 0; ulIdx < (DISPLAY_WIDTH); ulIdx++)
    {
    	pcBuf[ulIdx] = 0x00;
    }

	// Set Memory Addressing Mode (command = 1.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_MEMORYMODE);
	// to Page addressing mode (mode = 2.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_PAGEMODE);

    //
    // Move the display cursor to the first column of the specified row.
    //
    switch (ulY)
    {
    	case 0:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_0);
    		break;

    	case 1:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_1);
    		break;

    	case 2:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_2);
    		break;

    	case 3:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_3);
    		break;

    	case 4:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_4);
    		break;

    	case 5:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_5);
    		break;

    	case 6:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_6);
    		break;

    	case 7:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_7);
    		break;

    }

    //
    // write the buffer to the display to clear the selected row
    //
    I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, pcBuf, sizeof(pcBuf));

}

//*****************************************************************************
//
//! Clears a partial row on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param ulXStart is the starting column of the area to be cleared
//! \param ulXEnd is the end column of the area to be cleared
//! \param ulY is the display line to be cleared, 0 to 7.
//!
//! This function will clear a part of a row of the display.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1ClearArea(unsigned long ulI2CBase, unsigned long ulXStart,
			unsigned long ulXEnd, unsigned long ulY)
{
    char pcBuf[DISPLAY_WIDTH], ucIdx, ucCount;

    //
    // Calculate the size of the area to be cleared
    //
    ucCount = ulXEnd - ulXStart;

    //
    // Fill the buffer with zeros
    //
    for(ucIdx = 0; ucIdx < ucCount; ucIdx++)
    {
    	pcBuf[ucIdx] = 0x00;
    }

	// Set Memory Addressing Mode (command = 1.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_MEMORYMODE);
	// to Page addressing mode (mode = 2.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_PAGEMODE);

    //
    // Move the display cursor to the first column of the specified row.
    // Display Y-Position
    //
    switch (ulY)
    {
    	case 0:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_0);
    		break;

    	case 1:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_1);
    		break;

    	case 2:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_2);
    		break;

    	case 3:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_3);
    		break;

    	case 4:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_4);
    		break;

    	case 5:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_5);
    		break;

    	case 6:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_6);
    		break;

    	case 7:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_7);
    		break;

    }

    //
    // write the sequence of commands to move the cursor to the start of the
    // selected row
    //
    I2CWriteData(ulI2CBase, SSD_ADDR , CMDBYTE, g_pucSSDRowStart, sizeof(g_pucSSDRowStart));

	//
    // Move the display cursor to the start column of the specified area.
    // Display X-position
    //
    // Set lower column start address
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, ((ulXStart) & 0x0f));
    // Set higher column start address
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, (SSD1308_SETHIGHCOMPAGEMODE | (((ulXStart) >> 4) & 0x0f)));

    //
    // write the buffer to the display to clear the selected area
    //
    I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, pcBuf, ucCount);

}


//*****************************************************************************
//
//! Clears the OLED display.
//!
//! This function clears the OLED display, turning all pixels off.
//! \param ulI2CBase is the base for the I2C module.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1Clear(unsigned long ulI2CBase)
{
	Display128x64x1ClearLine(ulI2CBase, 0);
	Display128x64x1ClearLine(ulI2CBase, 1);
	Display128x64x1ClearLine(ulI2CBase, 2);
	Display128x64x1ClearLine(ulI2CBase, 3);
	Display128x64x1ClearLine(ulI2CBase, 4);
	Display128x64x1ClearLine(ulI2CBase, 5);
	Display128x64x1ClearLine(ulI2CBase, 6);
	Display128x64x1ClearLine(ulI2CBase, 7);
}

//*****************************************************************************
//
//! Initialize the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//!
//! This function configures the SSD1306 controller on the panel.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1Init(unsigned long ulI2CBase)
{
    //
    // Turn the display on.
    //
    Display128x64x1DisplayOn(ulI2CBase);

	//
    // Clear the frame buffer.
    //
    Display128x64x1Clear(ulI2CBase);
}


//*****************************************************************************
//
//! Displays a length-restricted string on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param pcStr is a pointer to the string to display.
//! \param ulLen is the number of characters to display.
//! \param ulX is the horizontal position to display the string, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to display the string, specified in
//! eight scan line blocks from the top of the display.
//!
//! This function will draw a specified number of characters of a string on the
//! display.  Only the ASCII characters between 32 (space) and 153 (inverted
//! question mark) are supported; other characters will result in random data
//! being draw on the display (based on whatever appears before/after the
//! font in memory).
//! The font is mono-spaced, so characters such as ``i'' and ``l'' have more white
//! space around them than characters such as ``m'' or ``w''.
//!
//! If the drawing of the string reaches the right edge of the display, no more
//! characters will be drawn.  Therefore, special care is not required to avoid
//! supplying a string that is ``too long'' to display.
//!
//! This function is similar to Display128x64x1StringDraw() except that the//
//! length of the string to display can be specified.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1StringDrawLen(unsigned long ulI2CBase, const char *pcStr, unsigned long ulLen,
                            unsigned long ulX, unsigned long ulY)
{
    unsigned char ucremainingcolumns;

	//
    // Check the arguments.
    //
    ASSERT(pcStr);
    ASSERT(ulX < DISPLAY_WIDTH);
    ASSERT(ulY < DISPLAY_NOOFPAGES);

	// Set Memory Addressing Mode (command = 1.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_MEMORYMODE);
	// to Page addressing mode (mode = 2.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_PAGEMODE);

	//
    // Move the display cursor to the first column of the specified row.
    // Display Y-position
    //
    switch (ulY)
    {
    	case 0:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_0);
    		break;

    	case 1:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_1);
    		break;

    	case 2:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_2);
    		break;

    	case 3:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_3);
    		break;

    	case 4:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_4);
    		break;

    	case 5:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_5);
    		break;

    	case 6:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_6);
    		break;

    	case 7:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_7);
    		break;

    }

	//
    // Move the display cursor to the first column of the specified column.
    // Display X-position
    //
    // Set lower column start address
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, ((ulX) & 0x0f));
    // Set higher column start address
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, (SSD1308_SETHIGHCOMPAGEMODE | (((ulX) >> 4) & 0x0f)));

    //
    // Loop while there are more characters in the string and the specified
    // length has not been exceeded.
    //
    while(ulLen && (*pcStr != 0))
    {
        //
        // See if there is enough space on the display for this entire
        // character.
        //
        if(ulX <= (DISPLAY_WIDTH - 6))
        {
            //
            // Write the contents of this character to the display.
            //
            I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, g_pucFont[*pcStr - ' '], 6);

            //
            // Increment the X coordinate by the five columns that were just
            // written.
            //
            ulX += 6;

            //
            // See if this is the last character to display (either because the
            // right edge has been reached or because there are no more
            // characters).
            //
            if(pcStr[1] == 0)
            {
                //
            	// The string has been displayed.
                //
                return;
            }
        }

        else
        {
        	ucremainingcolumns = (DISPLAY_WIDTH - ulX);

        	if (ucremainingcolumns == 0)
                //
                // The string has been displayed.
                //
                return;

        	else
        	{
        		if ((ucremainingcolumns - 1) > 0)
        		{
        			//
        			// Write the portion of the character that will fit onto the
        			// display.
        			//
        			I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, g_pucFont[*pcStr - ' '], (ucremainingcolumns-1));
        		}

        		else
        		{
        			//
        			// Write the portion of the character that will fit onto the
        			// display.
        			//
        			I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, g_pucFont[*pcStr - ' '], 1);
        		}
        		//
        		// The string has been displayed.
        		//
        		return;
        	}
        }

        //
        // Advance to the next character.
        //
        pcStr++;

        //
        // Decrement the character count
        //
        ulLen--;
    }
}

//*****************************************************************************
//
//! Displays a string on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param pcStr is a pointer to the string to display.
//! \param ulX is the horizontal position to display the string, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to display the string, specified in
//! eight scan line blocks from the top of the display.
//!
//! This function will draw a specified number of characters of a string on the
//! display.  Only the ASCII characters between 32 (space) and 153 (inverted
//! question mark) are supported; other characters will result in random data
//! being draw on the display (based on whatever appears before/after the
//! font in memory).
//! The font is mono-spaced, so characters such as ``i'' and ``l'' have more white
//! space around them than characters such as ``m'' or ``w''.
//!
//! If the drawing of the string reaches the right edge of the display, no more
//! characters will be drawn.  Therefore, special care is not required to avoid
//! supplying a string that is ``too long'' to display.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1StringDraw(unsigned long ulI2CBase, const char *pcStr, unsigned long ulX,
                         unsigned long ulY)
{
	unsigned long ulLen;

	//
    // Check the arguments.
    //
    ASSERT(pcStr);
    ASSERT(ulX < DISPLAY_WIDTH);
    ASSERT(ulY < DISPLAY_NOOFPAGES);

    //
    // How long is the supplied string?
    //
    ulLen = strlen(pcStr);

    //
    // Call the length restricted variant of this function, using a large
    // number for the length.
    //
    Display128x64x1StringDrawLen(ulI2CBase, pcStr, ulLen, ulX, ulY);
}

//*****************************************************************************
//
//! Draws a string horizontally centered on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param pcStr points to the NULL terminated string to be displayed.
//! \param ulY is the vertical position of the string specified in terms of
//!     8 pixel character cells.  Valid values are 0 and 1.
//! \param bClear is \b true if all uncovered areas of the display line are to
//!     be cleared or \b false if they are to be left unaffected.
//!
//! This function displays a string centered on a given line of the OLED
//! display and optionally clears sections of the display line to the left and
//! right of the provided string.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1StringDrawCentered(unsigned long ulI2CBase, const char *pcStr, unsigned long ulY,
                                 bool bClear)
{
    unsigned long ulLen, ulClip;

    //
    // Check the arguments.
    //
    ASSERT(pcStr);
    ASSERT(ulY < DISPLAY_NOOFPAGES);

    //
    // How long is the supplied string?
    //
    ulLen = strlen(pcStr);

    //
    // Is the string too wide to fit on the display?  If so, clip it left and
    // right to fit.
    //
    if(ulLen > 20)
    {
        ulClip = (ulLen - 20) / 2;
        ulLen = 20;
    }
    else
    {
        ulClip = 0;
    }

    //
    // If we've been asked to clear the background, clear it now.
    //
    if(bClear)
    {
        Display128x64x1ClearLine(ulI2CBase, ulY);
    }

    //
    // Now draw the string at the desired position.
    //
    Display128x64x1StringDrawLen(ulI2CBase, pcStr + ulClip, ulLen,
                                ((DISPLAY_WIDTH_HALF - (ulLen * 3))), ulY);
}

//*****************************************************************************
//
//! Displays an image on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param pucImage is a pointer to the image data.
//! \param ulX is the horizontal position to display this image, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to display this image, specified in
//! eight pages from the top of the display.
//!
//! This function will display a bitmap graphic on the display.
//!
//! The image data is organized with the first row of image data appearing left
//! to right, followed immediately by the second row of image data.  Each byte
//! contains the data for the eight scan lines of the column, with the top scan
//! line being in the least significant bit of the byte and the bottom scan
//! line being in the most significant bit of the byte.
//!
//! For example, an image four columns wide and sixteen scan lines tall would
//! be arranged as follows (showing how the eight bytes of the image would
//! appear on the display):
//!
//! \verbatim
//!     +-------+  +-------+  +-------+  +-------+
//!     |   | 0 |  |   | 0 |  |   | 0 |  |   | 0 |
//!     | B | 1 |  | B | 1 |  | B | 1 |  | B | 1 |
//!     | y | 2 |  | y | 2 |  | y | 2 |  | y | 2 |
//!     | t | 3 |  | t | 3 |  | t | 3 |  | t | 3 |
//!     | e | 4 |  | e | 4 |  | e | 4 |  | e | 4 |
//!     |   | 5 |  |   | 5 |  |   | 5 |  |   | 5 |
//!     | 0 | 6 |  | 1 | 6 |  | 2 | 6 |  | 3 | 6 |
//!     |   | 7 |  |   | 7 |  |   | 7 |  |   | 7 |
//!     +-------+  +-------+  +-------+  +-------+
//!
//!     +-------+  +-------+  +-------+  +-------+
//!     |   | 0 |  |   | 0 |  |   | 0 |  |   | 0 |
//!     | B | 1 |  | B | 1 |  | B | 1 |  | B | 1 |
//!     | y | 2 |  | y | 2 |  | y | 2 |  | y | 2 |
//!     | t | 3 |  | t | 3 |  | t | 3 |  | t | 3 |
//!     | e | 4 |  | e | 4 |  | e | 4 |  | e | 4 |
//!     |   | 5 |  |   | 5 |  |   | 5 |  |   | 5 |
//!     | 4 | 6 |  | 5 | 6 |  | 6 | 6 |  | 7 | 6 |
//!     |   | 7 |  |   | 7 |  |   | 7 |  |   | 7 |
//!     +-------+  +-------+  +-------+  +-------+
//! \endverbatim
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1ImageDraw(unsigned long ulI2CBase, const char *pucImage,
		unsigned long ulSize, unsigned long ulX, unsigned long ulY)
{
	//
	// Check the arguments.
	//
	ASSERT(ulX < DISPLAY_WIDTH);
	ASSERT(ulY < DISPLAY_NOOFPAGES);

	// Set Memory Addressing Mode (command = 1.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_MEMORYMODE);
	// to Page addressing mode (mode = 2.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_PAGEMODE);
	//
	// Move the display cursor to the first column of the specified column.
	// Display X-position
	//
	// Set lower column start address
	I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, (ulX & 0x0f));
	// Set higher column start address
	I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, (SSD1308_SETHIGHCOMPAGEMODE | ((ulX >> 4) & 0x0f)));

	//
	// Move the display cursor to the first column of the specified row.
	// Display Y-position
	//
	switch (ulY)
	{
		case 0:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_0);
			break;

		case 1:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_1);
			break;

		case 2:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_2);
			break;

		case 3:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_3);
			break;

		case 4:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_4);
			break;

		case 5:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_5);
			break;

		case 6:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_6);
			break;

		case 7:
			I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_7);
			break;
	}

	//
	// Write Image Data (Page) to Display.
	//
	I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, pucImage, ulSize);
}

//*****************************************************************************
//
//! Switches the OLED display to Double-Size Font.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param bSwitch with	\b true --> double size font switched on
//!     				\b false --> double size font switched off.
//!
//! This function uses an undocumented command of the SSD1308 OLED
//! controller to display each scan line of the first 4 text lines twice.
//! This is resulting in a font which is 2 pages tall.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1DoubleSizeFont(unsigned long ulI2CBase, bool bSwitch)
{
	//
	// Switch on/off Double Size Font (command = 1.byte)
	//
	I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_DISPLAY_FONT_DSIZE);

	if (bSwitch == 1)
		// ZOOM-ON (mode = 2.byte)
		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_DISPLAY_FONT_DSIZE_ON);

	else
		// ZOOM-OFF (mode = 2.byte)
		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_DISPLAY_FONT_DSIZE_OFF);
}

//*****************************************************************************
//
//! Switches the OLED display to Inverted Mode.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param bSwitch with	\b true --> inverted mode
//!     				\b false --> normal mode.
//!
//! It is assumed that display was initialized in NORMAL mode.
//! This function switches the OLED to the inverted mode or back to normal.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1Inverted(unsigned long ulI2CBase, bool bSwitch)
{
	//
	// Switch display to inverted mode
	//
	if (bSwitch == 1)
		// Inverted Mode
		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_DISPLAYINVERS);

	else
		// Normal Mode
		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_DISPLAYNORMAL);
}

//*****************************************************************************
//
//! Displays a length-restricted string on the OLED display.
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param pcStr is a pointer to the string to display.
//! \param ulLen is the number of characters to display.
//! \param ulX is the horizontal position to display the string, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to display the string, specified in
//! eight scan line blocks from the top of the display.
//!
//! This function is - from a functionality point of view - identical to
//! 'Display128x64x1StringDrawLen' except that it needs no X/Y-position
//! calling parameters.
//! Because of that, the function 'Display128x64x1SetCursor' needs to be
//! called before calling 'Display128x64x1StringCurrentPos'!
//!
//! This function is used by 'I2COLEDprintf', hence 'Display128x64x1StringCurrentPos'
//! needs to be called before using 'I2COLEDprintf' too.
//!
//! If the drawing of the string reaches the right edge of the display, no more
//! characters will be drawn.  Therefore, special care is not required to avoid
//! supplying a string that is ``too long'' to display.
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1StringCurrentPos(unsigned long ulI2CBase, const char *pcStr,
		unsigned long ulLen)
{
    unsigned char ucremainingcolumns;

    unsigned long ulX = 0;

    //
    // Loop while there are more characters in the string and the specified
    // length has not been exceeded.
    //
    while(ulLen && (*pcStr != 0))
    {
        //
        // See if there is enough space on the display for this entire
        // character.
        //
        if(ulX <= (DISPLAY_WIDTH - 6))
        {
            //
            // Write the contents of this character to the display.
            //
            I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, g_pucFont[*pcStr - ' '], 6);

            //
            // Increment the X coordinate by the five columns that were just
            // written.
            //
            ulX += 6;

            //
            // See if this is the last character to display (either because the
            // right edge has been reached or because there are no more
            // characters).
            //
            if(pcStr[1] == 0)
            {
                //
            	// The string has been displayed.
                //
                return;
            }
        }

        else
        {
        	ucremainingcolumns = (DISPLAY_WIDTH - ulX);

        	if (ucremainingcolumns == 0)
                //
                // The string has been displayed.
                //
                return;

        	else
        	{
        		if ((ucremainingcolumns - 1) > 0)
        		{
        			//
        			// Write the portion of the character that will fit onto the
        			// display.
        			//
        			I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, g_pucFont[*pcStr - ' '], (ucremainingcolumns-1));
        		}

        		else
        		{
        			//
        			// Write the portion of the character that will fit onto the
        			// display.
        			//
        			I2CWriteData(ulI2CBase, SSD_ADDR , DATBYTE, g_pucFont[*pcStr - ' '], 1);
        		}
        		//
        		// The string has been displayed.
        		//
        		return;
        	}
        }

        //
        // Advance to the next character.
        //
        pcStr++;

        //
        // Decrement the character count
        //
        ulLen--;
    }
}

//*****************************************************************************
//
//! Set the Cursor to the specified position on the OLED display (GDDRAM address).
//!
//! \param ulI2CBase is the base for the I2C module.
//! \param ulX is the horizontal position to move the cursor to, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to move the cursor to, specified in
//! eight scan line blocks from the top of the display
//!
//! This function sets the cursor to the specified GDDRAM position in the
//! OLED memory. It needs to be called prior calling 'I2COLEDprintf' or
//! 'Display128x64x1StringCurrentPos'!
//!
//! \return None.
//
//*****************************************************************************
void
Display128x64x1SetCursor(unsigned long ulI2CBase, unsigned long ulX, unsigned long ulY)
{
	//
    // Check the arguments.
    //
    ASSERT(ulX < DISPLAY_WIDTH);
    ASSERT(ulY < DISPLAY_NOOFPAGES);

	// Set Memory Addressing Mode (command = 1.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_MEMORYMODE);
	// to Page addressing mode (mode = 2.byte)
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_PAGEMODE);

	//
    // Move the display cursor to the first column of the specified row.
    // Display Y-position
    //
    switch (ulY)
    {
    	case 0:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_0);
    		break;

    	case 1:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_1);
    		break;

    	case 2:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_2);
    		break;

    	case 3:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_3);
    		break;

    	case 4:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_4);
    		break;

    	case 5:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_5);
    		break;

    	case 6:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_6);
    		break;

    	case 7:
    		I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, SSD1308_STARTPAGE_7);
    		break;

    }

	//
    // Move the display cursor to the specified column.
    // Display X-position
    //
    // Set lower column start address
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, ((ulX) & 0x0f));
    // Set higher column start address
    I2CRegWrite(ulI2CBase, SSD_ADDR, CMDBYTE, (SSD1308_SETHIGHCOMPAGEMODE | (((ulX) >> 4) & 0x0f)));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************






