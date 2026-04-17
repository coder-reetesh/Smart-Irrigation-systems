#include "stm32f4xx.h"
#include <stdio.h>
#define DRY_THRESHOLD 2000

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