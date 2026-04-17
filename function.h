#ifndef function_h_
#define function_h_

#include "stm32f407xx.h" 

/************************ADC************************/
void GPIO_INIT();
void ADC_INIT();
uint16_t ADC_READ();
/************************LCD************************/
void LCD_GPIO_init (void);
void delayMS(int delay);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);

#endif