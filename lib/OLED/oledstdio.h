//*****************************************************************************
//
// oledstdio.h - Header file for the OLED display Stdio Console
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
//*****************************************************************************

#ifndef OLEDSTDIO_H_
#define OLEDSTDIO_H_

//*****************************************************************************
//
// Parameter defines to which I2C bus the OLED for use as Stdio Console is
// connected to.
//
//*****************************************************************************
#define I2CCONSOLE				I2C0_BASE


//*****************************************************************************
//
// Prototypes for the driver APIs.
//
//*****************************************************************************
extern void I2COLEDprintf(const char *pcString, ...);

#endif /* OLEDSTDIO_H_ */
