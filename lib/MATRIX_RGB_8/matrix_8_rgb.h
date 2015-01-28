#ifndef MATRIX_8_RGB_H_
#define MATRIX_8_RGB_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
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
#include <driverlib/uart.h>
#include "driverlib/ssi.h"

#define LINE_PINS       GPIO_PIN_5+GPIO_PIN_6+GPIO_PIN_7
#define LINE_PORT       GPIO_PORTA_BASE

#define LAT_PIN         GPIO_PIN_5
#define LAT_PORT        GPIO_PORTE_BASE

#define BLANK_PIN       GPIO_PIN_5
#define BLANK_PORT      GPIO_PORTB_BASE

#define SCLK_PIN        GPIO_PIN_4
#define SCLK_PORT       GPIO_PORTB_BASE

#define MOSI_PIN        GPIO_PIN_7
#define MOSI_PORT       GPIO_PORTB_BASE

#define TIMES_TO_REPEAT_MATRIX 10 //Repeat 10 times until change image in the matrix

#define set_high(x,y)     GPIOPinWrite(x,y,y)
#define set_low(x,y)      GPIOPinWrite(x,y,~(y)) 
#define set_custom(x,y,z) GPIOPinWrite(x,y,z)

void ClkSSI();
bool SendSSITLC(unsigned char *matrix_t, uint16_t offset);
void CfgMatrixPins(void);
#endif
