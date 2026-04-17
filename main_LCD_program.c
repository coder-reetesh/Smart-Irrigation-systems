//	PC0 - PC7 = D0 - D7 // The data line of the LCD D0 to D7

//	PB5 = as the RS line (Register select)
//	PB6 = R/W  (Read / Write)
// PB7 = EN (Enable)

#include "stm32f4xx.h"

#define RS 0x20			//	0010 0000
#define RW 0x40			// 
#define EN (1U<<7)			// begening from line 1


/************************GPIO_init************************/
void LCD_GPIO_init (void)
{
	//RCC->AHB1ENR |= 0x06;
	RCC->AHB1ENR |= ((1<<1)|(1<<2));
	//GPIOB->MODER |= 0x5400;	
	GPIOB->MODER &=~(0x3F<<10); // Clear the PB5 - PB7
	GPIOB->MODER |=  (0x15 << 10);  // Set PB5–PB7 as output
	GPIOB->BSRR = 0x00c; // Set en RW low
	
	// Set PC0 - PC7 as output;
	GPIOC->MODER &= ~0x0000FFFF;   // Clear PC0–PC7
	GPIOC->MODER |=  0x00005555;   // Set PC0–PC7 as output

	//GPIOC -> MODER |= 0x00005555; // It set PC0 to PC7 as a output pin
	GPIOB->ODR &=(RS|RW|EN);
}


/************************delayMS************************/
void delayMS(int delay)
{
	int i;
	for(;delay>0;delay--)
	{
		for(i=0;i<3195;i++)
     		{}
	}
}
/************************LCD_command************************/
void LCD_command(unsigned char command)
{
	GPIOB->BSRR = EN;        // EN = 1
	delayMS(1);
	GPIOB->BSRR = EN << 16; // EN = 0
	delayMS(1);
	
	//	GPIOB->BSRR=0;
	GPIOB->BSRR =(RS | RW)<<16; //RS =0,R/W=0
	//GPIOB->BSRR|=EN;
	GPIOC->ODR =command;
	GPIOB->BSRR|=EN;
	//GPIOB->BSRR |=EN;
	delayMS(2);
	//GPIOB->BSRR &=~EN;
	GPIOB->BSRR |=EN<<16; //CLEAR EN;
}
/************************LCD_data************************/
void LCD_data(char data)
{
	//GPIOB->BSRR = 0;
	GPIOB->BSRR |=RS; //RS=1
	GPIOB->BSRR |= RW<<16; // RW=0;
	GPIOB->BSRR|=EN;
	GPIOC->ODR = data;
	
	delayMS(200);
	//GPIOB->BSRR &=~EN;
	GPIOB->BSRR|=EN<<16; // Clear EN
}
/************************LCD_init************************/
void LCD_init(void)
{
	LCD_GPIO_init();
	
	/*delayMS(30);
	LCD_command(1);
	delayMS(10);
	LCD_command(0x30);
	delayMS(1);
	LCD_command(0x30);
	*/
	
	 // Reset sequence (force 8-bit mode)
  LCD_command(0x30);
  delayMS(5);
  LCD_command(0x30);
  delayMS(1);
  LCD_command(0x30);

	
	LCD_command(0x38); //Set 8-bit data mode,2 line,5x7 font
	
	LCD_command(0x08);        // Display OFF
  LCD_command(0x01);        // Clear display
  delayMS(2);               // Required delay
	
	LCD_command(0x06); // Move cursor right
	LCD_command(0x01); // Clear screen , move cursor home
	delayMS(100);
	LCD_command(0x0F); // Turn display ON, blink cursor
}
int main()
{
	LCD_init();
	while(1)
	{
		LCD_command(0x80);
		LCD_data('S');
		LCD_data('O');
		LCD_data('I');
		LCD_data('L');
		LCD_data(' ');
		LCD_data('I');
		LCD_data('S');
		LCD_data(' ');
		LCD_data('D');
		LCD_data('R');
		LCD_data('Y');
		
		LCD_command(0x01); // Clear screen , move cursor home
		delayMS(500);
	}
}
