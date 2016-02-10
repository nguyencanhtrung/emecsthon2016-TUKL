/*
 * uart.h
 *
 *  Created on: 05/02/2016
 *      Author: hiber
 */

#ifndef UART_H_
#define UART_H_
#include <stdint.h>

extern volatile uint8_t rx_data[2];
extern volatile uint8_t mode;

void usart_enable_rx_isr(void);
void usart_send_data(uint8_t data);
void usart_send_string(char *str);
void usart_init(void);


#endif /* UART_H_ */
