#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>

#define RS (1U<<5) 	//	PB5
#define RW (1U<<6) 	//PB6
#define EN (1U<<7)	//PB7

#define GPIOBEN			(1<<1)
#define GPIOCEN			(1<<2)
/************************delayMs************************/
void delayMS(int delay)
{
	int i;
	for(;delay>0;delay--)
	{
		for(i=0;i<3195;i++)
     		{}
	}
}
/************************GPIO_init************************/
void lcd_GPIO_init(void)
{
	RCC->AHB1ENR |=(GPIOBEN | GPIOCEN); //	Enable clock
	GPIOB->MODER &=~(0x3F<<10);					//	Clear PB5-PB7
	GPIOB->MODER |=(0x15<<10);					//	Set Output mode
	GPIOC->MODER &=~(0x0000FFFF);				//	Clear PC0-PC7
	GPIOC->MODER |=(0x00005555);				//	Output mode
	GPIOB->ODR &= ~(RS | RW | EN);      // Clear control pins
}
/************************lcd_cmd_write************************/
void lcd_cmd_write(char cmd)
{
	RCC->AHB1ENR |=(GPIOBEN | GPIOCEN); // Enable clock
	GPIOB->ODR &= ~(RS | RW);   // RS=0, RW=0 (Command mode)
	//GPIOB->ODR &=~RW;
	GPIOC->ODR = cmd;
	//GPIOC->ODR|=cmd<<15;
	//GPIOB->ODR&=~RS;
	GPIOB->ODR|=EN;
	delayMS(2);
	GPIOB->ODR&=~EN;
}
/************************lcd_dat_write************************/
void lcd_dat_write(uint8_t dat)
{
	GPIOB->ODR |= RS;           // RS = 1 (Data mode)
	GPIOB->ODR&=~RW;
	GPIOC->ODR = dat;           // Send data to data bus
	//GPIOB->ODR|=dat<<15;
	//GPIOB->ODR&=~RS;
	GPIOB->ODR|=EN;
	delayMS(2);
	GPIOB->ODR&=~EN;
}

/************************lcd_init************************/
void lcd_init(void)
{
	lcd_GPIO_init();
	delayMS(10);
	//GPIOC->MODER |=RS|RW|EN;
	lcd_cmd_write(0x30);
  delayMS(5);
  lcd_cmd_write(0x30);
  delayMS(1);
  lcd_cmd_write(0x30);
	//delayMS(2);
	lcd_cmd_write(0x38);
	lcd_cmd_write(0x08);
	delayMS(2);
	lcd_cmd_write(0x06);
	
	//lcd_cmd_write(0x01);
	delayMS(2);
	lcd_cmd_write(0x0F);
}
/************************lcd_str_write************************/
void lcd_str_write(char *str)
{
	while(*str)
	{
		lcd_dat_write(*str++);
	}
}

int main()
{
	lcd_init();
	lcd_cmd_write(0x01);
	lcd_cmd_write(0x80);
	lcd_str_write("Soil is dry..");
	while(1){}
}
