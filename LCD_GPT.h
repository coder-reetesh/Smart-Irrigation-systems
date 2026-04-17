#ifndef _LCD_GPT_H
#define _LCD_GPT_H


void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void LCD_EnablePulse(void);
void LCD_Command(unsigned char cmd);
void LCD_Data(unsigned char data);
void LCD_Init(void);
void LCD_String(char *str);
void GPIO_Init(void);

#endif