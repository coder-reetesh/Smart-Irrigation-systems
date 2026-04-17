#include "stm32f4xx.h"
#include <stdint.h>

#define RS (1U<<5)   // PB5
#define RW (1U<<6)
#define EN (1U<<7)   // PB7

#define GPIOBEN (1U<<1)
#define GPIOCEN (1U<<2)

/**************** Delay ****************/
void delayMS(int ms)
{
    while ( ms--){
			 volatile int i =0;
			while(i<60000) i++;
		}
}

/**************** GPIO Init ****************/
void lcd_GPIO_init(void)
{
    RCC->AHB1ENR |= GPIOBEN | GPIOCEN;

    volatile uint32_t dummy =RCC->AHB1ENR;
		GPIOB->MODER &= ~(0x3F << 10);
    GPIOB->MODER |=  (0x15 << 10);   // PB5, PB7 output
		

		//GPIOB->MODER &= ~(0xFF << 10);     // Clear PB5,6,7
		//GPIOB->MODER |=  (0x55 << 10);     // Set PB5,6,7 as output
		
		/* For 8-bit mode
    GPIOC->MODER &= ~0x0000FFFF;
    GPIOC->MODER |=  0x00005555;     // PC0–PC7 output
		*/
		// Configure PC4, PC5, PC6, PC7 as outputs (For LCD D4, D5, D6, D7)
		GPIOC->MODER &= ~(0xFF << 8);    // Clear MODER bits for PC4-PC7
    GPIOC->MODER |=  (0x55 << 8);    // Set them to general purpose output mode
	
    GPIOB->ODR &= ~(RS | EN |RW);
}

/**************** lcd_pulse_enable ****************/

void lcd_pulse_enable(void)
{
		GPIOB->ODR &= ~EN;
		delayMS(1);
		GPIOB->ODR |= EN;
    delayMS(1);
    GPIOB->ODR &= ~EN;
}

/**************** lcd_write_nibble ****************/

void lcd_write_nibble(uint8_t nibble)
{
	/*
	 // Clear the current data on PC4-PC7
    GPIOC->ODR &= ~(0xF << 4);
    
    // Place the new 4-bit nibble onto PC4-PC7
    GPIOC->ODR |= (nibble << 4);
    */
		GPIOC->ODR = (GPIOC->ODR & ~(0xF << 4)) | ((nibble & 0x0F) << 4);
    // Pulse the enable pin to tell LCD to read the data
    lcd_pulse_enable();
}

/**************** Command ****************/
void lcd_cmd(uint8_t cmd)
{
    GPIOB->ODR &= ~RS;
	 GPIOB->ODR &= ~RW;   // ADD THIS LINE
    //GPIOC->ODR = cmd;
		//GPIOC->ODR = (GPIOC->ODR & 0xFF00) | cmd;
    //lcd_pulse_enable();
		
		lcd_write_nibble(cmd>>4);
		lcd_write_nibble(cmd & 0x0f);
		delayMS(2);		
}

/**************** Data ****************/
void lcd_data(uint8_t data)
{
    GPIOB->ODR |= RS;
    //GPIOC->ODR = data;
		//GPIOC->ODR = (GPIOC->ODR & 0xFF00) | data;
    //lcd_pulse_enable();
		lcd_write_nibble(data>>4);
		lcd_write_nibble(data & 0xf);
		delayMS(1);
}

/**************** Init ****************/
void lcd_init(void)
	{
    lcd_GPIO_init();
    delayMS(100);     // Wait for LCD to power up (>40ms)
		
		GPIOB->ODR &= ~RS;   // Ensure RS is low to send initialization instructions

    // 4-Bit Initialization Sequence (Mandatory "wake up" sequence for HD44780)
    
		lcd_write_nibble(0x03); //change //previous lcd_write_nibble(0x03); 
    delayMS(10);            // Wait > 4.1ms
    lcd_write_nibble(0x03); 
    delayMS(1);            // Wait > 100us
    lcd_write_nibble(0x03); 
    delayMS(1);
    
    lcd_write_nibble(0x02); // Switch to 4-bit mode!
    delayMS(5);
		
		/*lcd_cmd(0x030);
    delayMS(10);
    lcd_cmd(0x30);
    delayMS(5);
    lcd_cmd(0x30);
		delayMS(1);*/
		
 // Now we can use the standard 8-bit split functions
 
		lcd_cmd(0x28);       // Function set: 4-bit mode, 2 lines, 5x8 font
    lcd_cmd(0x0f);       // Display ON, Cursor OFF, Blink OFF
    lcd_cmd(0x01);       // Clear Display
    delayMS(20);          // Clearing display takes a long time
    lcd_cmd(0x06);       // Entry mode set: Increment cursor automatically
	
}

/**************** String ****************/
void lcd_print(char *s)
{
    while(*s != '\0')
        lcd_data(*s++);
}

/**************** MAIN ****************/
int main(void)
{
		SystemCoreClockUpdate();
    lcd_init();
    /*lcd_cmd(0x80);
		lcd_print("Test...");
		lcd_cmd(0xC0);
		//lcd_cmd(0x80);
    lcd_print("Soil dry...");
		lcd_cmd(0x01);*/
	lcd_cmd(0x80);  
		lcd_print("WELCOME TO MY");
	lcd_cmd(0xC0);
	lcd_print("HELLISH WORLD");
	lcd_cmd(0x01);
	lcd_cmd(0x80);  
		lcd_cmd(0x80);  
		lcd_print("MY NAME IS ");
	lcd_cmd(0xC0);
	lcd_print("REETESH (*_*)");
		while(1){};
}
