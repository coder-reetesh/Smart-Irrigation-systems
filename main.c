#include "stm32f4xx.h"
#include "function.h"
int analogvalue;

int main()
{
	RCC->AHB1ENR |=1;
	GPIOA->MODER |= 0Xc; //SET PA1 AS A ANALOG PIN
		
	RCC->APB2ENR |= 0X100; //ENABLE CLOCK ACCESS TO ADC
	ADC1->CR2=0;	//DISABLE ADC
	ADC1->SQR3 =1;
	ADC1->CR2|=1;		//ENABLE ADC
	
	
	while(1)
	{
		ADC1 ->CR2 |=0X40000000;
		while((ADC1->CR2 & 2)){}
		analogvalue= ADC1->DR;
	}
}