/*
 * uart.c
 *
 *  Created on: 05/02/2016
 *      Author: hiber
 */
#include "uart.h"
#include "efm32gg990f1024.h"
#include "em_usart.h"
#include "em_gpio.h"

volatile uint8_t rx_data[2];
uint8_t temp;
volatile uint8_t mode;

#define COM_PORT	gpioPortD //USART 1
#define TX_PIN		0
#define RX_PIN		1

void usart_init(void) {
	CMU->HFPERCLKEN0 |= (1 << 13) | (1 << 1);        // Enable GPIO, and USART1 peripheral clocks

	GPIO->P[COM_PORT].MODEL = (1 << 4) | (4 << 0);  // Configure PD0 as digital output and PD1 as input
	GPIO->P[COM_PORT].DOUTSET = (1 << TX_PIN); // Initialize PD0 high since UART TX idles high (otherwise glitches can occur)

	// Use default value for USART1->CTRL: asynch mode, x16 OVS, lsb first, CLK idle low
	// Default frame options: 8-none-1-none
	USART1->CLKDIV = (152 << 6);                               // 152 will give 38400 baud rate (using 16-bit oversampling with 24MHz peripheral clock)
	USART1->CMD = (1 << 11) | (1 << 10) | (1 << 2) | (1 << 0); // Clear RX/TX buffers and shif regs, Enable Transmitter and Receiver
	USART1->IFC = 0x1FF9;                                      // clear all USART interrupt flags
	USART1->ROUTE = 0x103;                                     // Enable TX and RX pins, use location #1 (UART TX and RX located at PD0 and PD1, see EFM32GG990 datasheet for details)
	mode = 0x01;
	rx_data[0]= 0x09;
	rx_data[1]= 0x09;
}

void usart_enable_rx_isr(void) {
	USART1->IEN = USART_IEN_RXDATAV;
	NVIC_EnableIRQ(USART1_RX_IRQn);
}

void usart_send_data(uint8_t data) {
	USART_Tx( USART1, data);
}

void usart_send_string(char *str) {
	int i = 0;
	for(i = 0; str[i] != '\0'; i++) {
		usart_send_data(str[i]);
	}
}

void USART1_RX_IRQHandler(void) {
	if(USART1->STATUS & (1 << 7)) {   // if RX buffer contains valid data   -- first if
		temp= USART1->RXDATA;      // store the data
		switch(temp){
		case 0xff:
			mode = 0x02;
			break;
		default:
			mode = 0x01;
			if((temp & 0x40)==0){
				rx_data[0]=temp;
			} else {
				if((temp & 0x40)==0x40){
					rx_data[1]=(temp & 0xBf);
				}
				if(USART1->STATUS & (1 << 6)) { // check if TX buffer is empty
					USART1->TXDATA = rx_data[0];     // echo received char
				}
			}
		}
	}
}

