//*****************************************************************************
//
// display128x64x1.h - Header file for the 128x64 monochrome graphical OLED
//                    display driver (based on SSD1306 controller)
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE AUTHOR SHALL NOT, UNDER
// ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// AUTHOR: JOERG QUINTEN
// E2E-NICKNAME: aBUGSworstnightmare
//
// Revision: r03
//
//*****************************************************************************

#ifndef DISPLAY128X64X1_H_
#define DISPLAY128X64X1_H_

//*****************************************************************************
//
// The I2C slave address of the SSD controller on the OLED display.
//
//*****************************************************************************
#define SSD_ADDR		0x3c


//*****************************************************************************
//
// Command Byte definition of the SSD controller on the OLED display.
//
//*****************************************************************************
//
// The byte transferred after this command byte will be interpreted as command
// by the SSD controller
//
#define CMDBYTE			0x80

//
// The byte transferred after this command byte will be interpreted as data
// by the SSD controller
//
#define DATBYTE			0x40


//*****************************************************************************
//
//! The number of pixel across the display.
//
//*****************************************************************************
#define DISPLAY_WIDTH		128
#define DISPLAY_WIDTH_HALF	64
#define DISPLAY_HEIGHT		64
#define DISPLAY_NOOFPAGES	8


//*****************************************************************************
//
//! The width of a character cell in pixels.  This applies to the font embedded
//! within the display driver.
//
//*****************************************************************************
#define CHAR_CELL_WIDTH 6


//*****************************************************************************
//
//! The number of characters that can be printed on a single line of the
//! display assuming a 6 pixel wide character cell.
//
//*****************************************************************************
#define CHARS_PER_LINE (DISPLAY_WIDTH / CHAR_CELL_WIDTH)


//*****************************************************************************
//
//! SSD1308 Command Table
//! Fundamental Commands
//
//*****************************************************************************
// Set Lower Column Start Address for Page Addressing Mode
// This command is only for Page Addressing Mode
#define SSD1308_SETLOWCOMPAGEMODE		0x00
// Set Higher Column Start Address for Page Addressing Mode
// This command is only for Page Addressing Mode
#define SSD1308_SETHIGHCOMPAGEMODE		0x10

// Set Memory Addressing Mode - 2-byte Command
#define SSD1308_MEMORYMODE				0x20
// Horizontal Addressing Mode
#define SSD1308_HORIZONTALMODE			0x00
// Vertical Addressing Mode
#define SSD1308_VERTICALMODE			0x01
// Page Addressing Mode ((RESET))
#define SSD1308_PAGEMODE				0x02

// Set Column Address - 3-byte Command
// This Command is only for Horizontal or Vertical Mode
#define SSD1308_COLUMNADDRESS			0x21
// (RESET) values for Column Start Address (STARTRST)
// and Column End Address (ENDRST)
// 2.byte; Start Address
#define SSD1308_COLUMNADDRESS_STARTRST	0x00
// 3.byte: End Address
#define SSD1308_COLUMNADDRESS_ENDRST	0x7F

// Set Page Address - 3-byte Command
// This Command is only for Horizontal or Vertical Mode
#define SSD1308_PAGEADDRESS				0x22
// (RESET) values for Page Start Address (STARTRST)
// and Page End Address (ENDRST)
#define SSD1308_PAGEADDRESS_STARTRST	0x00
#define SSD1308_PAGEADDRESS_ENDRST		0x07


// Set Display Start Line (RESET)
#define SSD1308_DISPLAYSTARTLINE_RST	0x40
// Set Display Start Line to 32d (0x20) to display
// Page4 to Page7 when using Double-Size Font
// Command
#define SSD1308_DISPLAYSTARTLINE_DSIZE	0x60

// Set Display Contrast Control - 2-byte Command
#define SSD1308_DISPLAYCONTRAST			0x81
// select one out of 256
// (RESET) = 0x7F
#define SSD308_DISPLAYCONTRASTRST		0x7F
// contrast for wide.hk 0.96in OLED
#define SSD308_DISPLAYCONTRASTWHK		0xAF

// Set Segment Re-Map
// column address 0 is mapped to SEG0 (RESET)
// I2C interface wires on bottom of wide.hk 0.96in OLED
#define SSD1308_DISPLAYMAPRST			0xA0
// column address 127 is mapped to SEG0
// I2C interface wires on top of wide.hk 0.96in OLED
#define SSD1308_DISPLAYMAP				0xA1

// Entire Display ON
// Resume to RAM content display (RESET)
// Output follows RAM content
#define SSD1308_DISPLAYALLONRESUME		0xA4
// Entire Display ON
// Output ignores RAM content
#define SSD1308_DISPLAYALLON			0xA5

// Set Normal/Inverse Display
// Normal display (RESET)
// -> 0 in RAM: OFF in display panel
// -> 1 in RAM: ON in display panel
#define SSD1308_DISPLAYNORMAL			0xA6
#define SSD1308_DISPLAYINVERS			0xA7

// Set Multiplex Ratio
#define SSD1308_SETMULTIPLEX			0xA8
// 1/64 Multiplex (RESET)
#define SSD1308_MULTIRATIO				0x3F

// External or internal IREF Selection
#define SSD1308_DISPLAYIREF				0xAD
// -> 0x00: External IREF (RESET)
#define SSD1308_DISPLAYIREFEXT			0x00
// -> 0x40: Enable internal IREF during display ON
#define SSD1308_DISPLAYIREFINT			0x10

// Set Display ON/OFF
// Display OFF (sleep mode) (RESET)
#define SSD1308_DISPLAYOFF				0xAE
// Display ON in Normal Mode
#define SSD1308_DISPLAYON				0xAF

// Set Page Start Address for Page Addressing Mode
// this command is only for page addressing mode
#define SSD1308_STARTPAGE_0				0xB0
#define SSD1308_STARTPAGE_1				0xB1
#define SSD1308_STARTPAGE_2				0xB2
#define SSD1308_STARTPAGE_3				0xB3
#define SSD1308_STARTPAGE_4				0xB4
#define SSD1308_STARTPAGE_5				0xB5
#define SSD1308_STARTPAGE_6				0xB6
#define SSD1308_STARTPAGE_7				0xB7

// Set COM Output Scan Direction
// Normal Mode (RESET) - scan from COM0 to COM[n-1]
#define SSD1308_COMDIRECTIONRST			0xC0
// Remapped mode - scan from COM[n-1] to COM0
#define SSD1308_COMDIRECTIONMAP			0xC8

// Set Display Offset - 2-byte Command
// Set vertical shift by COM from 0d to 63d
#define SSD1308_DISPLAYOFFSET			0xD3
// no offset (RESET)
#define SSD1308_DISPLAY_NOOFFSET		0x00

// Set Display Clock Divide Ratio/Oscillator Frequency - 2-byte Command
#define SSD1308_DISPLAYCLOCKDIV			0xD5
// (RESET) value - refer to display data sheet for custom settings
#define SSD1308_CLOCKDIVRST				0x80

// Set Pre-charge period - 2-byte Command
#define SSD1308_PRECHARGE				0xD9
// Period (RESET) - refer to display data sheet for custom settings
#define SSD1308_PRECHARGEPERIOD			0x22

// Set COM Pins Hardware configuration - 2-byte Command
#define SSD1308_COMPINCONFIG			0xDA
// (RESET): Disable Left/right remap, Alternative COM pin config
#define SSD1308_COMPINCONFIGRST			0x12
// Enable Left/right remap, Sequential COM pin config
#define SSD1308_COMPINCONFIGMAP			0x22

// Set Vcomh Deselect Level - 2-byte Command
#define SSD1308_SETVCOMHDESELECT		0xDB
// Vcomh deselect level ~0.65 x Vcc
#define SSD1308_COMHLEVEL065			0x00
// Vcomh deselect level ~0.77 x Vcc (RESET)
#define SSD1308_COMHLEVEL077			0x20
// Vcomh deselect level ~0.83 x Vcc
#define SSD1308_COMHLEVEL083			0x30

// Command for No Opetration NOP
#define SSD1308_NOP						0xE3


//*****************************************************************************
//
//! SSD1308 Command Table
//! Scrolling Commands
//
//*****************************************************************************
// Continuous Horizontal Scroll Setup - 7-byte Command
// Horizontal scroll by 1 column
// 1.byte: direction (left/right)
#define SSD1308_SCROLL_HOR_RIGHT		0x26
#define SSD1308_SCROLL_HOR_LEFT			0x27

// 2.byte: Dummy byte FALSE
#define SSD1308_DUMMYFALSE				0x00

// Start Page/End Page Address
// 3.byte: Start Page Address
#define SSD1308_SCROLL_0				0x00
#define SSD1308_SCROLL_1				0x01
#define SSD1308_SCROLL_2				0x02
#define SSD1308_SCROLL_3				0x03
#define SSD1308_SCROLL_4				0x04
#define SSD1308_SCROLL_5				0x05
#define SSD1308_SCROLL_6				0x06
#define SSD1308_SCROLL_7				0x07

// 4.byte: Set time interval between each frame scroll step in terms of frame frequency
#define SSD1308_SCROLLSPEED_5			0x00
#define SSD1308_SCROLLSPEED_64			0x01
#define SSD1308_SCROLLSPEED_128			0x02
#define SSD1308_SCROLLSPEED_256			0x03
#define SSD1308_SCROLLSPEED_3			0x04
#define SSD1308_SCROLLSPEED_4			0x05
#define SSD1308_SCROLLSPEED_25			0x06
#define SSD1308_SCROLLSPEED_2			0x07

// 5.byte: End Page Address

// 6.byte: DUMMY byte FALSE

// 7.byte: DUMMY byte TRUE
#define SSD1308_DUMMYTRUE				0xFF

// Continuous Vertical Scroll Setup - 6-byte Command
// 1.byte: direction (left/right)
// Vertical and right Horizontal scroll by 1 column
#define SSD1308_SCROLL_VERT_RIGHT		0x29
// Vertical and left Horizontal scroll by 1 column
#define SSD1308_SCROLL_VERT_LEFT		0x2A
// please note: no continious vertical scrolling available!

// 2.byte: Dummy byte FALSE

// 3.byte: Start Page Address

// 4.byte: Set time interval between each frame scroll step in terms of frame frequency

// 5.byte: End Page Address

// Deactivate Scroll
// After stopping the scrolling, the RAM data needs to be rewritten
#define SSD1308_SCROLL_STOP 			0x2E

// Activate Scroll
#define SSD1308_SCROLL_START 			0x2F

// Set vertical scroll area - 3-byte Command
#define SSD1308_SET_VERTICAL 			0xA3
// 2.byte: Set Number of Rows in Top Fixed Area (RESET = 0)
#define SSD1308_ROWSFIXED_RST			0x00
// 3.byte: Set Number of Rows in Scroll Area (RESET = 64)
// refer to display data sheet for details
#define SSD1308_ROWSSCROLL_RST			0x40


//*****************************************************************************
//
//! SSD1308 Command Table
//! Power Supply Configuration
//
//*****************************************************************************
// Charge Pump Setting - 2-byte Command
// the Charge Pump must be enabled by the following sequence of commands:
// SSD1308_CHARGEPUMP, SSD1308_CHARGEPUMP_ON, SSD1308_DISPLAYON
#define SSD1308_CHARGEPUMP 				0x8D
// Disable Charge Pump (RESET)
#define SSD1308_EXTERNALVCC 			0x10
// Enable Charge Pump during display on
#define SSD1308_CHARGEPUMP_ON			0x14

//*****************************************************************************
//
//! SSD1308 Display Font Size Double
//! Undocumented Command from SSD1306: 0xD6
//
//*****************************************************************************
#define SSD1308_DISPLAY_FONT_DSIZE		0xD6
// ON --> Displays each scan line of the first 4 text lines twice
// --> Font size is doubled
#define SSD1308_DISPLAY_FONT_DSIZE_ON	0x01
// OFF
// --> font size is normal
#define SSD1308_DISPLAY_FONT_DSIZE_OFF	0x00


//*****************************************************************************
//
// Prototypes for the driver APIs.
//
//*****************************************************************************
extern void Display128x64x1Clear(unsigned long ulI2CBase);

extern void Display128x64x1ClearLine(unsigned long ulI2CBase,
									unsigned long ulY);

extern void Display128x64x1ClearArea(unsigned long ulI2CBase,
									unsigned long ulXStart,
									unsigned long ulXEnd,
									unsigned long ulY);

extern void Display128x64x1StringDraw(unsigned long ulI2CBase,
									const char *pcStr,
									unsigned long ulX,
									unsigned long ulY);

extern void Display128x64x1StringDrawLen(unsigned long ulI2CBase,
									const char *pcStr,
									unsigned long ulLen,
                                    unsigned long ulX,
                                    unsigned long ulY);

extern void Display128x64x1StringDrawCentered(unsigned long ulI2CBase,
									const char *pcStr,
									unsigned long ulY,
									bool bClear);

extern void Display128x64x1ImageDraw(unsigned long ulI2CBase,
									const char *pucImage,
									unsigned long ulSize,
									unsigned long ulX,
									unsigned long ulY);

extern void Display128x64x1Init(unsigned long ulI2CBase);

extern void Display128x64x1DisplayOn(unsigned long ulI2CBase);

extern void Display128x64x1DisplayOff(unsigned long ulI2CBase);

extern void Display128x64x1DoubleSizeFont(unsigned long ulI2CBase,
									bool bSwitch);

extern void Display128x64x1Inverted(unsigned long ulI2CBase,
									bool bSwitch);

extern void Display128x64x1StringCurrentPos(unsigned long ulI2CBase,
									const char *pcStr,
									unsigned long ulLen);

extern void Display128x64x1SetCursor(unsigned long ulI2CBase,
									unsigned long ulX,
									unsigned long ulY);

#endif /* DISPLAY128X64X1_H_ */
