/****************************************************************************

	███████╗   ███████╗   ██████╗
	██╔════╝   ██╔════╝   ██╔══██╗
	█████╗     ███████╗   ██████╔╝
	██╔══╝     ╚════██║   ██╔═══╝
	███████╗██╗███████║██╗██║
	╚══════╝╚═╝╚══════╝╚═╝╚═╝

    LCD - API for use the LCD 16X2
    Author: Anderson Ignacio da Silvav
    Date: 04/07/2014
    Target:PART_TM4C123GH6PM
    Inf.: http://www.esplatforms.blogspot.com.br


	Pin connections:
	
	   [LCD]  [ARM]
		RS => PB5
		RW => GND
		EN => PB0

		D4 => PD0
		D5 => PD1
		D6 => PD2
		D7 => PD3

	To Initialize:

		lcd_init();
		lcd_clear();

	To deslocate the cursor:

		goto_lcd(x,y);
		
****************************************************************************/

#ifndef SYSCTL_H
#define SYSCTL_H
	#include "driverlib/sysctl.h"
#endif
#ifndef GPIO_H
#define GPIO_H
	#include "driverlib/gpio.h"
#endif

#define  lcd_rs		GPIO_PIN_5
#define  lcd_en		GPIO_PIN_0
#define  DB4		GPIO_PIN_0
#define  DB5		GPIO_PIN_1
#define  DB6		GPIO_PIN_2
#define  DB7		GPIO_PIN_3
#define  LED		GPIO_PIN_2
#define  PORT_RS_EN	GPIO_PORTB_BASE
#define  PORT_DATA	GPIO_PORTD_BASE

uint8_t	pos=0;

typedef enum
	{
	LCD_LINE_1,
	LCD_LINE_2
	}LCD_LINE;

void delay_lcd(long delayms)
{
	SysCtlDelay( (SysCtlClockGet()/(3*1000))*delayms) ;
}

void lcd_cmd(int cmd)
{
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_rs,0x00);
	GPIOPinWrite(GPIO_PORTD_BASE,DB4|DB5|DB6|DB7,(cmd>>4));
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0xff);
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0x00);
	GPIOPinWrite(GPIO_PORTD_BASE,DB4|DB5|DB6|DB7,(cmd));
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0xff);
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0x00);
	delay_lcd(1);
}

void lcd_data(char cmd)
{
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_rs,0xff);
	GPIOPinWrite(GPIO_PORTD_BASE,DB4|DB5|DB6|DB7,(cmd>>4));
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0xff);
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0x00);
	GPIOPinWrite(GPIO_PORTD_BASE,DB4|DB5|DB6|DB7,(cmd));
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0xff);
	GPIOPinWrite(GPIO_PORTB_BASE,lcd_en,0x00);
	delay_lcd(1);
}


void lcd_clear()
{
	delay_lcd(50);
	lcd_cmd(0x01);
	delay_lcd(50);
}

void lcd_init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	delay_lcd(1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	delay_lcd(1);
	GPIOPinTypeGPIOOutput(PORT_DATA, DB4 | DB5 | DB6 | DB7);
	delay_lcd(1);
	GPIOPinTypeGPIOOutput(PORT_RS_EN, lcd_rs | lcd_en | LED);
	
	delay_lcd(300);
	lcd_cmd(0x28);//Ajusta LCD para 2 linhas com uma matriz de 5x7 (4 bits)
	delay_lcd(5);
	lcd_cmd(0x28);
	delay_lcd(5);
	lcd_cmd(0x28);
	delay_lcd(5);
	lcd_cmd(0x28);
	delay_lcd(5);
	lcd_cmd(0x0c);//Display acesso sem cursor
	delay_lcd(5);
	lcd_cmd(0x06);//Escreve deslocando o cursor para a direita
	delay_lcd(5);
	lcd_cmd(0x01);//Limpa display e retorna o cursor para o inicio
	delay_lcd(5);

}

void goto_lcd(int x,int y)
{
	if(y==1)
	{
		lcd_cmd(0x80+x-1);
	}
	else 	if(y==2)
			{
				lcd_cmd(0xC0+x-1);
			}
}

void lcd_string(char *c/*,LCD_LINE L*/)
{
	while(*c)	lcd_data(*(c++));
}

