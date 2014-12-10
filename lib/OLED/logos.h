//*****************************************************************************
//
// logos.h - Logo Data File for use with Driver for the 128x64 OLED
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
//*****************************************************************************
// C3ii-Logo
// c3iitag_size is the output file from 'LCDAssistant.exe'.
// The array is re-formatted to represent its 8 pages (height).
//
static const char c3iitag_size [] =
{
		//
		// first 4 bytes hold information on image size
		// size in pixels width (2 bytes; LSB, MSB) - size in pixels height (2 bytes; LSB, MSB)
		//
		//0x49, 0x00, 0x40, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x78, 0x78,
		0x3C, 0xBC, 0x78, 0x7C, 0x7C, 0x7C, 0xFC, 0xF8, 0xF0, 0xF0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xF8, 0xFC,
		0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xF0, 0xFB, 0x7F, 0x1F, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x7F, 0x7E, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x7F, 0x7E, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x00,

		0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x03, 0x03, 0x03, 0x03,
		0x01, 0x01, 0x07, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x03, 0x03, 0x03, 0x03, 0x07, 0x1F, 0x1F, 0x3F, 0xFE, 0xFC, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x1E, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3E, 0xFF, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xC0, 0xE0, 0x78, 0x70, 0x3F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x02,

		0x00, 0x00, 0x40, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x06,
		0x03, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,

		0x00, 0x00, 0x00, 0x03, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xF3, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0F, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFC, 0xF8, 0x80,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x03, 0x7F, 0xFF, 0x3F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x03, 0x7F, 0xFF, 0x3F, 0x0F, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F, 0x0F,
		0x1F, 0x1E, 0x3C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
};

//
// C3ii-Logo - single pages
// These array will be written to the OLED in Page Address Mode
//
static const char c3iitag_page0 [] =
{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x78, 0x78,
		0x3C, 0xBC, 0x78, 0x7C, 0x7C, 0x7C, 0xFC, 0xF8, 0xF0, 0xF0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const char c3iitag_page1 [] =
{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xF8, 0xFC,
		0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xF0, 0xFB, 0x7F, 0x1F, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x7F, 0x7E, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x7F, 0x7E, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x00
};

static const char c3iitag_page2 [] =
{
		0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x03, 0x03, 0x03, 0x03,
		0x01, 0x01, 0x07, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x03, 0x03, 0x03, 0x03, 0x07, 0x1F, 0x1F, 0x3F, 0xFE, 0xFC, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const char c3iitag_page3 [] =
{
		0x00, 0x00, 0x1E, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x3E, 0xFF, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xC0, 0xE0, 0x78, 0x70, 0x3F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x02
};

static const char c3iitag_page4 [] =
{
		0x00, 0x00, 0x40, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x06,
		0x03, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

static const char c3iitag_page5 [] =
{
		0x00, 0x00, 0x00, 0x03, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xF3, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x00
};

static const char c3iitag_page6 [] =
{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0F, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFC, 0xF8, 0x80,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x03, 0x7F, 0xFF, 0x3F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x03, 0x7F, 0xFF, 0x3F, 0x0F, 0x00, 0x00, 0x00
};

static const char c3iitag_page7 [] =
{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F, 0x0F,
		0x1F, 0x1E, 0x3C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
};

//
// C3ii-Projects Page QRcode
// The array is re-formatted to represent its 5 pages (height).
//
static const char qrcode_size [] =
{
		//
		// first 4 bytes hold information on image size
		// size in pixels witdh (2 bytes; LSB, MSB) - size in pixels height (2 bytes; LSB, MSB)
		//
		//0x29, 0x00, 0x29, 0x00,

		0x00, 0x00, 0xFC, 0x04, 0x74, 0x74, 0x74, 0x04, 0xFC, 0x00, 0xA8, 0xE0, 0x2C, 0x80, 0xB4, 0xA0,
		0x00, 0x58, 0x48, 0x24, 0x54, 0x64, 0xD4, 0x58, 0x78, 0x74, 0x60, 0xA0, 0x6C, 0x88, 0xD4, 0x00,
		0xFC, 0x04, 0x74, 0x74, 0x74, 0x04, 0xFC, 0x00, 0x00,

		0x00, 0x00, 0xB5, 0xCD, 0xAD, 0x6D, 0x95, 0xA1, 0x55, 0x8C, 0x8D, 0xE6, 0xEB, 0x06, 0x69, 0xB8,
		0xA3, 0xFA, 0xAD, 0x54, 0x31, 0xAE, 0xCB, 0xBA, 0x8F, 0xEC, 0xC5, 0xDC, 0x75, 0x50, 0x9B, 0xF4,
		0x51, 0x3D, 0x71, 0x65, 0xD9, 0x85, 0x11, 0x00, 0x00,

		0x00, 0x00, 0xF8, 0xD4, 0xD9, 0x37, 0x97, 0xE6, 0x55, 0x41, 0x61, 0x86, 0x5F, 0x07, 0x69, 0x33,
		0x62, 0xF5, 0x70, 0xD4, 0xB3, 0xE8, 0xAE, 0xAB, 0xA9, 0x8A, 0x12, 0x4C, 0x55, 0x51,	0xCB, 0xF4,
		0x45, 0xBB, 0x76, 0xA4, 0x45, 0xD8, 0x77, 0x00, 0x00,

		0x00, 0x00, 0x7A, 0x04, 0x77, 0x1B, 0x09, 0x39, 0x55, 0x6D, 0xF8, 0x83, 0x19, 0x07, 0x69, 0x38,
		0x50, 0x7C, 0x0B, 0x5D, 0xB8, 0xAE, 0x9B, 0x33, 0x74, 0x8A, 0x46, 0x54, 0xCF, 0xB5, 0xD9, 0x4F,
		0x77, 0x69, 0xF5, 0x65, 0xC5, 0x1D, 0x37, 0x00, 0x00,

		0x00, 0x00, 0x7F, 0x41, 0x5D, 0x5D, 0x5D, 0x41, 0x7F, 0x00, 0x7D, 0x36, 0x47, 0x07, 0x69, 0x38,
		0x5A, 0x75, 0x6D, 0x54, 0x18, 0x2E, 0x4B, 0x2B, 0x66, 0x6E, 0x47, 0x54,	0x4C, 0x24, 0x37, 0x64,
		0x0D, 0x24, 0x4F, 0x51, 0x45, 0x57, 0x79, 0x00, 0x00

		// although the input .bmp file was exactly 41x41pixels, 'LCDAssistent.exe' generated an output
		// array which is 5 bytes larger than expected --> tweak your output file if necessary!
		// , 0x00, 0x00, 0x00, 0x00, 0x00,
};

//
// C3ii-Projects Page QRcode
// These array will be written to the OLED in Page Address Mode
//
static const char qrcode_page0 [] =
{
		0x00, 0x00, 0xFC, 0x04, 0x74, 0x74, 0x74, 0x04, 0xFC, 0x00, 0xA8, 0xE0, 0x2C, 0x80, 0xB4, 0xA0,
		0x00, 0x58, 0x48, 0x24, 0x54, 0x64, 0xD4, 0x58, 0x78, 0x74, 0x60, 0xA0, 0x6C, 0x88, 0xD4, 0x00,
		0xFC, 0x04, 0x74, 0x74, 0x74, 0x04, 0xFC, 0x00, 0x00
};

static const char qrcode_page1 [] =
{
		0x00, 0x00, 0xB5, 0xCD, 0xAD, 0x6D, 0x95, 0xA1, 0x55, 0x8C, 0x8D, 0xE6, 0xEB, 0x06, 0x69, 0xB8,
		0xA3, 0xFA, 0xAD, 0x54, 0x31, 0xAE, 0xCB, 0xBA, 0x8F, 0xEC, 0xC5, 0xDC, 0x75, 0x50, 0x9B, 0xF4,
		0x51, 0x3D, 0x71, 0x65, 0xD9, 0x85, 0x11, 0x00, 0x00
};

static const char qrcode_page2 [] =
{
		0x00, 0x00, 0xF8, 0xD4, 0xD9, 0x37, 0x97, 0xE6, 0x55, 0x41, 0x61, 0x86, 0x5F, 0x07, 0x69, 0x33,
		0x62, 0xF5, 0x70, 0xD4, 0xB3, 0xE8, 0xAE, 0xAB, 0xA9, 0x8A, 0x12, 0x4C, 0x55, 0x51,	0xCB, 0xF4,
		0x45, 0xBB, 0x76, 0xA4, 0x45, 0xD8, 0x77, 0x00, 0x00
};

static const char qrcode_page3 [] =
{
		0x00, 0x00, 0x7A, 0x04, 0x77, 0x1B, 0x09, 0x39, 0x55, 0x6D, 0xF8, 0x83, 0x19, 0x07, 0x69, 0x38,
		0x50, 0x7C, 0x0B, 0x5D, 0xB8, 0xAE, 0x9B, 0x33, 0x74, 0x8A, 0x46, 0x54, 0xCF, 0xB5, 0xD9, 0x4F,
		0x77, 0x69, 0xF5, 0x65, 0xC5, 0x1D, 0x37, 0x00, 0x00
};

static const char qrcode_page4 [] =
{
		0x00, 0x00, 0x7F, 0x41, 0x5D, 0x5D, 0x5D, 0x41, 0x7F, 0x00, 0x7D, 0x36, 0x47, 0x07, 0x69, 0x38,
		0x5A, 0x75, 0x6D, 0x54, 0x18, 0x2E, 0x4B, 0x2B, 0x66, 0x6E, 0x47, 0x54,	0x4C, 0x24, 0x37, 0x64,
		0x0D, 0x24, 0x4F, 0x51, 0x45, 0x57, 0x79, 0x00, 0x00
};
