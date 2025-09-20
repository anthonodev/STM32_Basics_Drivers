/*
 * uart.h
 *
 *  Created on: May 1, 2024
 *      Author: User
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "stm32f4xx.h"

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);
void uart2_tx_init(void);
void uart2_rxtx_init(void);
char uart2_read(void);

#define SR_RXNE			(1U<<5)
#define HISR_TCIF6		(1U<<21)
#define HIFCR_CTCIF6	(1U<<21)

#endif /* UART_H_ */
