#include "stm32f4xx.h"

// #include "function.h"


#define GPIOBEN			(1U<<1)
#define TIM4EN 			(1U<<2)
#define TIM4_CCR1 	(1U<<4)
#define CC1S_IP_TI1	(1U<<0)
#define CC2S_IP_TI1 (2U<<8)


volatile uint32_t msTicks = 0;
/*
void SystemClock_Config_168MHz(void) {
    // 1. Enable HSE
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY)); // Wait for HSE

    // 2. Configure Flash (5 wait states for 168MHz)
    FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

    // 3. Configure PLL settings (Assuming 8MHz HSE)
    // M=8, N=336, P=2, Q=7
    RCC->PLLCFGR = (8U) | (336U << 6) | (((2U >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (7U << 24);

    // 4. Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));


    // 5. Setup Prescalers (AHB=1, APB1=4, APB2=2)
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

    // 6. Switch to PLL
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}
*/
/********************GPIO_init********************/
void GPIO_init(void)
{
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER &=~(3U<<12);

	GPIOB->MODER |=(2U<<12);

	GPIOB->AFR[0] &= ~(0xF << 24);

	GPIOB->AFR[0] |= (0x2 << 24);
}

/********************PWM_init********************/
void PWM_init()
{
		RCC->APB1ENR |= TIM4EN;

		TIM4->PSC = 16 - 1;

    TIM4->ARR = 20000 - 1;
	
		TIM4->CNT = 0;
	
		TIM4->CCMR1 &= ~(7U << 4);
	
		TIM4->CCMR1 |= (6U << 4);   // PWM mode 1 on CH1
	
		TIM4->CCMR1 |= (1U << 3); // Enable preload
	
    //TIM4->CCER |= (0U << 1);
	
		TIM4->CCER |= (1U << 0);   // ? ENABLE CHANNEL 1
		
		TIM4->CCR1 = 1500;
	
		TIM4->EGR |= (1U << 0);    // ? FORCE UPDATE EVENT
		
		//TIM4->CR1 |= (1U << 7);     // ARPE
    TIM4->CR1 |= (1U << 0);     // CEN ? START TIMER
}
/********************delay_ms********************/
void delay_ms(uint32_t ms)
{
    while ( ms--){
			 int i =0;
			while(i<60000) i++;
		}
}
/********************************************************************************/


int main(void) {
    // 1. Initialize hardware
	//	SystemClock_Config_168MHz();
    SystemCoreClockUpdate();
		GPIO_init(); // Configure PB6 as AF2
    PWM_init(); // Setup TIM4 for 50Hz PWM
    	

    while (1) {
        // 0 Degrees (~1ms pulse)
        TIM4->CCR1 = 500;
        delay_ms(50);

        //90 Degrees (~1.5ms pulse)
        TIM4->CCR1 = 1500;
        delay_ms(50);

        // 180 Degrees (~2ms pulse)
        //TIM4->CCR1 = 2000;
        //delay_ms(50);
			
				TIM4->CCR1 = 2500;
        delay_ms(50);
			
			/*TIM4->CCR1 = 2000;
        delay_ms(50);
			
			TIM4->CCR1 = 2000;
        delay_ms(50);*/
    }
}