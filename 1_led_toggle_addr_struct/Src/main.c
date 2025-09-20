//Where is the led connected?
//Port: PA5 or PB13
//Pin:  21  or 34
//AHB1 -> GPIOA -> 0x4002 0000 - 0x4002 03FF
//AHB1 -> GPIOB -> 0x4002 0400 - 0x4002 07FF
//AHB1 -> RCC	-> 0x4002 3800 - 0x4002 3BFF RCC
//AHB1
#include <stdint.h>

#define PERIPH_BASE 		(0x40000000UL)
#define AHB1PERIPH_OFFSET	(0x00020000UL)
#define AHPB1PERIPH_BASE	(PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET		(0x0000UL)
#define GPIOA_BASE			(AHPB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			(0x3800UL)
#define RCC_BASE			(AHPB1PERIPH_BASE + RCC_OFFSET)



#define GPIOAEN				(1U<<0) // 0b 0000 0000 0000 0000 0000 0000 0000 0001

#define PIN5				(1U<<5)
#define LED_PIN				PIN5

//---------------
#define __IO volatile

typedef struct
{
	volatile uint32_t DUMMY[12];
	volatile uint32_t AHB1ENR;
}RCC_TypeDef;

typedef struct
{
	volatile uint32_t MODER;   /*!< GPIO port mode register,		        Address offset: 0x00 */
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR;     /*!< GPIO port output data register,		    Address offset: 0x14 */
}GPIO_TypeDef;

#define RCC   ((RCC_TypeDef*) RCC_BASE)
#define GPIOA ((GPIO_TypeDef*) GPIOA_BASE)

//---------------

int main(void)
{
	//1. Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;
	//2. Set PA5 as output pin
	GPIOA->MODER |=  (1U<<10);// Set bit 10 to 1
	GPIOA->MODER &=~ (1U<<11);//Set bit 11 to 0

	while(1)
	{
		//3. Set PA5 high
		//GPIOA->ODR |= LED_PIN;
		//4. Experiment: toggle PA5
		GPIOA->ODR ^= LED_PIN;
		for(int i=0;i<100000;i++){}

	}
}
