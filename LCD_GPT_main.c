#include "stm32f4xx.h"

/* Control Pins */
#define RS (1<<5)   // PB5
#define RW (1<<6)   // PB6
#define EN (1<<7)   // PB7

/* Function Prototypes */
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void LCD_EnablePulse(void);
void LCD_Command(unsigned char cmd);
void LCD_Data(unsigned char data);
void LCD_Init(void);
void LCD_String(char *str);
void GPIO_Init(void);

int main(void)
{
    GPIO_Init();
    LCD_Init();

    LCD_String("STM32F407VG");
    LCD_Command(0xC0);   // Move to 2nd line
    LCD_String("LCD 8-bit Mode");

    while(1);
}
/*******************************************************************************************/
void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1<<2);  // Enable GPIOC clock
    RCC->AHB1ENR |= (1<<1);  // Enable GPIOB clock

    /* PC0-PC7 as output (LCD Data) */
    GPIOC->MODER = 0x00005555;

    /* PB5, PB6, PB7 as output (RS, RW, EN) */
    GPIOB->MODER |= (1<<10) | (1<<12) | (1<<14);
}
/*******************************************************************************************/
void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1<<2);  // Enable GPIOC clock
    RCC->AHB1ENR |= (1<<1);  // Enable GPIOB clock

    /* PC0-PC7 as output (LCD Data) */
    GPIOC->MODER = 0x00005555;

    /* PB5, PB6, PB7 as output (RS, RW, EN) */
    GPIOB->MODER |= (1<<10) | (1<<12) | (1<<14);
}
/*******************************************************************************************/
void delay_us(uint32_t us)
{
    for(uint32_t i=0;i<us*16;i++);
}

void delay_ms(uint32_t ms)
{
    for(uint32_t i=0;i<ms*16000;i++);
}
/*******************************************************************************************/
void LCD_EnablePulse(void)
{
    GPIOB->ODR |= EN;
    delay_us(10);
    GPIOB->ODR &= ~EN;
    delay_us(100);
}

/*******************************************************************************************/
void LCD_Command(unsigned char cmd)
{
    GPIOB->ODR &= ~RS;   // RS = 0
    GPIOB->ODR &= ~RW;   // RW = 0

    GPIOC->ODR = cmd;    // Put command on data bus
    LCD_EnablePulse();
}

/*******************************************************************************************/
void LCD_Data(unsigned char data)
{
    GPIOB->ODR |= RS;    // RS = 1
    GPIOB->ODR &= ~RW;   // RW = 0

    GPIOC->ODR = data;   // Put data on data bus
    LCD_EnablePulse();
}
/*******************************************************************************************/
void LCD_Init(void)
{
    delay_ms(20);

    LCD_Command(0x38);  // 8-bit, 2 line, 5x7 font
    LCD_Command(0x0C);  // Display ON, Cursor OFF
    LCD_Command(0x06);  // Auto increment cursor
    LCD_Command(0x01);  // Clear display
    delay_ms(2);
}

/*******************************************************************************************/
void LCD_String(char *str)
{
    while(*str)
    {
        LCD_Data(*str++);
    }
}
