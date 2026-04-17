#include "function.h"

/************************ADC************************/
void GPIO_INIT()
{
	RCC->AHB1ENR |=1;
	GPIOA->MODER |= 0Xc; //SET PA1 AS A ANALOG PIN
		
	RCC->APB2ENR |= 0X100; //ENABLE CLOCK ACCESS TO ADC
}

void ADC_INIT()
{
	ADC1->CR2=0;	//DISABLE ADC
	ADC1->SQR3 =1;
	ADC1->CR2|=1;		//ENABLE ADC
}

uint16_t ADC_READ()
{
	while(1)
	{
		ADC1->CR2 |=0X40000000;
		while((ADC1->CR2 & 2)){}
		return ADC1->DR;
	}
}
/************************LCD************************/
void LCD_GPIO_init (void)
{
	RCC->AHB1ENR |= 0x06;
	GPIOB->MODER |= 0x5400;	
	
	GPIOB->BSRR = 0x00c; // Set en RW low
	
	// Set PC0 - PC7 as output;
	GPIOC -> MODER |= 0x00005555; // It set PC0 to PC7 as a output pin	
}

void delayMS(int delay)
{
	int i;
	for(;delay>0;delay--)
	{
		for(i=0i<3195;i++)
		{}
	}
}

void LCD_command(unsigned char command)
{
	GPIOB ->BSRR = (RS | RW)<<16; //RS =0,R/W=0
	GPIOC ->ODR =command;
	GPIOB->BSRR =EN;
	delayMS(0);
	GPIO->BSRR =EN<<16; //CLEAR EN;
}
void LCD_data(char data)
{
	GPIOB ->BSRR =RS; //RS=1
	GPIOB -> BSRR = RW<<16; // RW=0;
	GPIOC -> ODR = data;
	GPIOB ->BSRR =EN;
	delayMS(0);
	GPIOB ->BSRR EN<<16; // Clear EN
}
void LCD_init(void)
{
	LCD_GPIO_init();
	
	delayMs(30);
	LCD_command(0x30);
	delayMs(10);
	LCD_command(0x30);
	delayMs(1);
	LCD_command(0x30);
	
	LCD_command(0x38); //Set 8-bit data mode,2 line,5x7 font
	LCD_command(0x06); // Move cursor right
	LCD_command(0x01); // Clear screen , move cursor home
	LCD_command(0x0F); // Turn display ON, blink cursor
}