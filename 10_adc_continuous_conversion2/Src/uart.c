/*
 * uart.c
 *
 *  Created on: May 1, 2024
 *      Author: User
 */
//Define section
#include "uart.h"

#define GPIOAEN (1U<<0)
#define UART2EN (1U<<17)

#define CR1_TE (1U<<3)
#define CR1_RE (1U<<2)

#define CR1_UE (1U<<13)
#define SR_TXE (1U<<7)
#define SR_RXNE (1U<<5)

#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200

//Functions signature
void uart2_rxtx_init(void);
void uart2_write(int ch);
char uart2_read(void);
static void uart_set_baudrate(USART_TypeDef *USARTTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t BaudRate);

//overwrite function IO_putchar redirecting printf to uart
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}
//Functions
void uart2_rxtx_init(void)
{
	/*Configure uart gpio pin*/
	//Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;
	//Set PA2 mode to alternate function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	//Set PA2 alternate function type to UART TX (AF07) => AFR[0] = AFRL, AFR[1] = AFRH
	GPIOA->AFR[0] |= ((1U<<8) | (1U<<9) | (1U<<10));
	GPIOA->AFR[0] &=~(1U<<11);
	//Set PA3 mode to alternate function mode
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	//Set PA3 alternate function type to UART RX (AF07) => AFR[0] = AFRL, AFR[1] = AFRH
	GPIOA->AFR[0] |= ((1U<<12) | (1U<<13) | (1U<<14));
	GPIOA->AFR[0] &=~(1U<<15);

	/*Configure uart gpio pin*/
	//Enable clock access to UART2
	RCC->APB1ENR |= UART2EN;
	//Configure the UART baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	//configure the transfer direction
	USART2->CR1 = CR1_TE | CR1_RE; //do not use the | operator to clear all bits which are not TE.
	//Enable UART module
	USART2->CR1 |= CR1_UE;//do not want to clean register so we keep with | operator. Here activate UART
}

char uart2_read(void)
{
	//Make sure the receive data register is not empty
	while(!(USART2->SR & SR_RXNE)){}
	/*Read data*/
	return USART2->DR;
}

void uart2_write(int ch)
{
	//Make sure the transmit data register is empty
	while(!(USART2->SR & SR_TXE)){}
	//write to transmit data register
	USART2->DR = ch & 0XFF;
}

static void uart_set_baudrate(USART_TypeDef *USARTTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTTx->BRR = compute_uart_div(PeriphClk, BaudRate);
}

static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}

