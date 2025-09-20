#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "tim.h"

#define GPIOAEN	(1U<<0)
#define PIN5	(1U<<5)

#define LED_PIN	PIN5

//Main
int main(void)
{
	tim2_output_compare();
	while(1)
	{
	}
}






