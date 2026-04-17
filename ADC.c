#include "stm32f4xx.h"
#include <stdio.h>
#define DRY_THRESHOLD 2000

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
int main()
{
	GPIO_INIT();
	ADC_INIT();
	
	while(1)
	{
		uint16_t moisture=ADC_READ();
		if (moisture<DRY_THRESHOLD)
		{
			
		}
		else
		{
			
		}
	}
}