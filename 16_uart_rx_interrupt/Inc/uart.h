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

#define SR_RXNE (1U<<5)
void uart2_rx_interrupt_init(void);
char uart2_read(void);

#endif /* UART_H_ */
