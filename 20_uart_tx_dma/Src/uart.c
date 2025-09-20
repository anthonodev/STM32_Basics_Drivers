#include "uart.h"

#define GPIOAEN (1U<<0)
#define UART2EN (1U<<17)

#define CR1_TE (1U<<3)
#define CR1_RE (1U<<2)

#define CR1_UE (1U<<13)
#define SR_TXE (1U<<7)

#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200

//DMA variables
#define DMA1EN					(1U<<21)
#define DMA_S_EN 				(1U<<0)
#define CHSEL4					(1U<<27)
#define DMA_MEM_INC 			(1U<<10)
#define DMA_DIR_MEM_TO_PERIPH	(1U<<6)
#define DMA_CR_TCIE				(1U<<4)
#define DMA_CR_EN				(1U<<0)
#define UART_CR3_DMAT			(1U<<7)

//Functions signature
void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);
void uart2_tx_init(void);
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
void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len)
{
	/*Enable clock access to DMA*/
	RCC->AHB1ENR |= DMA1EN;
	/*Disable DMA1 Stream6*/
	DMA1_Stream6->CR &=~ DMA_S_EN;
	/*Wait until DMA1 Stream6 is disabled*/
	while(DMA1_Stream6->CR & DMA_S_EN){}
	/*Clear all interrupt flags of stream6*/
	DMA1->HIFCR |=(1U<<16);
	DMA1->HIFCR |=(1U<<18);
	DMA1->HIFCR |=(1U<<19);
	DMA1->HIFCR |=(1U<<20);
	DMA1->HIFCR |=(1U<<21);
	/*Set the destination buffer*/
	DMA1_Stream6->PAR = dst;
	/*Set the source buffer*/
	DMA1_Stream6->M0AR = src;
	/*Set length*/
	DMA1_Stream6->NDTR = len;
	/*Select Stream6 CH4 (use equal sign = to clean register)*/
	DMA1_Stream6->CR = CHSEL4;
	/*Enable memory increment (Set it to 1 implies memory address pointer is incremented after each data transfer "increment is done according to MSIZE")*/
	DMA1_Stream6->CR |= DMA_MEM_INC;
	/*Configure transfer direction*/
	DMA1_Stream6->CR |= DMA_DIR_MEM_TO_PERIPH;
	/*Enable DMA transfer complete interrupt*/
	DMA1_Stream6->CR |= DMA_CR_TCIE;
	/*Enable direct mode and disable FIFO*/
	DMA1_Stream6->FCR = 0;
	/*Enable DMA1 Stream6*/
	DMA1_Stream6->CR |= DMA_CR_EN;
	/*Enable UART2 transmitter DMA*/
	USART2->CR3 |= UART_CR3_DMAT;
	/*DMA Interrupt enable in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void uart2_tx_init(void)
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

	/*Configure uart gpio pin*/
	//Enable clock access to UART2
	RCC->APB1ENR |= UART2EN;
	//Configure the UART baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	//configure the transfer direction
	USART2->CR1 = CR1_TE; //do not use the | operator to clear all bits which are not TE.
	//Enable UART module
	USART2->CR1 |= CR1_UE;//do not want to clean register so we keep with | operator. Here activate UART
}

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

