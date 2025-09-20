#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "systick.h"

#define GPIOAEN	(1U<<0)
#define PIN5	(1U<<5)

#define LED_PIN	PIN5

static void systick_callback(void);

//Main
int main(void)
{
	//1. Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;
	//2. Set PA5 as output pin
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	systick_1hz_interrupt();

	while(1)
	{
	}
}

static void systick_callback(void)
{
	GPIOA->ODR ^= LED_PIN;
}

void SysTick_Handler(void)
{
	//Do something
	systick_callback();
}




