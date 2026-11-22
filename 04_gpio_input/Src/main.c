#include "stm32f4xx.h"

#define GPIOAEN (1U<<0)
#define GPIOCEN (1U<<2)
#define PIN5    (1U<<5)
#define PIN13   (1U<<13)
#define LED_PIN PIN5
#define BTN_PIN PIN13

int main(void)
{
	/*Enable clock access to GPIOA and GPIOC*/
	RCC->AHB1ENR |= (GPIOAEN | GPIOCEN);

	/*Set PA5 as output pin*/
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);

	/*Set PC13 as output pin*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

	while(1)
	{
		/*Check if B1 user button is pressed*/
		if (GPIOC->IDR & BTN_PIN)
		{
			/*Turn ON Led*/
			GPIOA->BSRR = LED_PIN;
		}
		else
		{
			/*Turn OFF Led*/
			GPIOA->BSRR = (1U<<21);
		}
	}
}
