/****************************************************************************
    ███████╗   ███████╗   ██████╗
    ██╔════╝   ██╔════╝   ██╔══██╗
    █████╗     ███████╗   ██████╔╝
    ██╔══╝     ╚════██║   ██╔═══╝
    ███████╗██╗███████║██╗██║
    ╚══════╝╚═╝╚══════╝╚═╝╚═╝

    SERVO MOTOR - API
    Author: Anderson Ignacio da Silva
    Date: 08/08/2014
    Target:PART_TM4C123GH6PM
    Inf.: http://www.esplatforms.blogspot.com.br

    PWM Pins:
    Channels  |   Pin
       0          PC4
       1          PC5
       2          PC6
       3          PC7
       4          PD0
       5          PD1

    Instructions:
        -To initialize just call InitServo()
        -Change the duty with SendServo(Channel,0-180 degrees)
        -If your servo has a different duty period, you can ch
         ange the time definitions in "servo_api.h"
        -Remember to change the MIN and MAX duty of each servo

****************************************************************************/

#ifndef SERVO_API_H
#define SERVO_API_H
	#include "servo_api.h"
#endif
// #ifndef SERVO_API_H
// #define SERVO_API_H
// 	#include "servo_api.c"
// #endif

/*  Definitions       */
#define PWM_FREQUENCY   50
#define PWM_PERIOD      20000  //Time in us

/* Some times for popular motors 
    -Time for max PWM servo (us)   MG995=2100 &  HS422=2300
    -Time for min PWM servo (us)   MG995=500  &  HS422=800 
*/

#define MAX_SERVO_0     1900
#define MIN_SERVO_0      500

#define MAX_SERVO_1     1900
#define MIN_SERVO_1      500

#define MAX_SERVO_2     1900
#define MIN_SERVO_2      500

#define MAX_SERVO_3     1900
#define MIN_SERVO_3      500

#define MAX_SERVO_4     1900
#define MIN_SERVO_4      500

#define MAX_SERVO_5     1900
#define MIN_SERVO_5      500

#define MAX_SERVO_5     1900
#define MIN_SERVO_5      500
//#define DEBUG_SERVO

/*  Prototypes  */
void SendServo(uint8_t SelServo,uint8_t ValServo);
void CfgModPWM(void);
void CalcServo(void);
void InitServo(void);
