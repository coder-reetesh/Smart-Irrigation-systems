#include "stm32f4xx.h"
#include <stdint.h>

#define RS (1U<<5)   // PB5
#define RW (1U<<6)   // PB6
#define EN (1U<<7)   // PB7

#define GPIOBEN (1<<1)
#define GPIOCEN (1<<2)

/************* Small delay for EN pulse *************/
static void EN_pulse(void)
{
    GPIOB->ODR |= EN;
    for(volatile int i=0;i<50;i++);   // ~short delay
    GPIOB->ODR &= ~EN;
}

/************* ms delay *************/
void delayMS(int delay)
{
    for(int d=0; d<delay; d++)
        for(volatile int i=0;i<3195;i++);
}

/************* GPIO init *************/
void lcd_GPIO_init(void)
{
    RCC->AHB1ENR |= (GPIOBEN | GPIOCEN);

    // PB5,6,7 as output
    GPIOB->MODER &= ~(0x3F << 10);
    GPIOB->MODER |=  (0x15 << 10);

    // PC0-PC7 as output
    GPIOC->MODER &= ~(0x0000FFFF);
    GPIOC->MODER |=  (0x00005555);

    GPIOB->ODR &= ~(RS | RW | EN);
}

/************* Command write *************/
void lcd_cmd_write(uint8_t cmd)
{
    GPIOB->ODR &= ~(RS | RW);   // Command mode

    GPIOC->ODR = (GPIOC->ODR & 0xFF00) | cmd;

    EN_pulse();
    delayMS(2);
}

/************* Data write *************/
void lcd_dat_write(uint8_t dat)
{
    GPIOB->ODR |= RS;     // Data mode
    GPIOB->ODR &= ~RW;

    GPIOC->ODR = (GPIOC->ODR & 0xFF00) | dat;

    EN_pulse();
    delayMS(2);
}

/************* LCD init *************/
void lcd_init(void)
{
    lcd_GPIO_init();
    delayMS(50);

    lcd_cmd_write(0x38);   // 8-bit, 2 line
    lcd_cmd_write(0x0C);   // Display ON, cursor OFF
    lcd_cmd_write(0x06);   // Entry mode
    lcd_cmd_write(0x01);   // Clear display
    delayMS(5);
}

/************* String write *************/
void lcd_str_write(char *str)
{
    while(*str)
        lcd_dat_write(*str++);
}

/************* Main *************/
int main(void)
{
    lcd_init();

    lcd_cmd_write(0x80);   // First line
    lcd_str_write("Soil is dry..");

    while(1){}
}
