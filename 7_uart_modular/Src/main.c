#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

//Main
int main(void)
{
	//Initialise UART
	uart2_tx_init();
	while(1)
	{
		printf("Hello from STM32F4.......\n\r");
	}
}





