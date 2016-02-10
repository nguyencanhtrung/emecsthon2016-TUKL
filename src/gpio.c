/*
 * gpio.c
 *
 *  Created on: Feb 5, 2016
 *      Author: trungnguyen
 */
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_chip.h"
#include <stdbool.h>

volatile bool gpio_data_x[8], gpio_data_y[8];

void gpioInit(void)
{
	/* Enable clock for GPIO module */
	  CMU_ClockEnable(cmuClock_GPIO, true);

	/* Configure INPUT pins for infrared LED */
	  GPIO_PinModeSet(gpioPortD, 3, gpioModeInput, 0);	// PD3
	  GPIO_PinModeSet(gpioPortD, 4, gpioModeInput, 0);  // PD4
	  GPIO_PinModeSet(gpioPortD, 5, gpioModeInput, 0);	// PD5
	  GPIO_PinModeSet(gpioPortD, 6, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortD, 7, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortD, 8, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortD, 13, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortD, 14, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortD, 15, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);	// PB9
	  GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortB, 11, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortB, 12, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortC, 0, gpioModeInput, 0);	// PA12
	  GPIO_PinModeSet(gpioPortC, 1, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortC, 2, gpioModeInput, 0);

	 /* Configure Input pin interrupt on falling edge */
	  //GPIO_IntConfig(gpioPortD, 3, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 4, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 5, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 6, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 7, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 8, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 13, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 14, false, true, true);
	  //GPIO_IntConfig(gpioPortD, 15, false, true, true);
	  //GPIO_IntConfig(gpioPortB, 9, false, true, true);
	  //GPIO_IntConfig(gpioPortB, 10, false, true, true);
	  //GPIO_IntConfig(gpioPortB, 11, false, true, true);
	  //GPIO_IntConfig(gpioPortB, 12, false, true, true);
	  //GPIO_IntConfig(gpioPortC, 0, false, true, true);
	  //GPIO_IntConfig(gpioPortC, 1, false, true, true);
	  //GPIO_IntConfig(gpioPortC, 2, false, true, true);
}

void readGpioX(void)
{
	gpio_data_x[0] = GPIO_PinInGet(gpioPortB, 10);
	gpio_data_x[1] = GPIO_PinInGet(gpioPortB, 9);
	gpio_data_x[2] = GPIO_PinInGet(gpioPortB, 11);
	gpio_data_x[3] = GPIO_PinInGet(gpioPortB, 12);
	gpio_data_x[4] = GPIO_PinInGet(gpioPortD, 15);
	gpio_data_x[5] = GPIO_PinInGet(gpioPortD, 3);
	gpio_data_x[6] = GPIO_PinInGet(gpioPortD, 4);
	gpio_data_x[7] = GPIO_PinInGet(gpioPortD, 5);
}

void readGpioY(void)
{
	gpio_data_y[0] = GPIO_PinInGet(gpioPortC, 0);
	gpio_data_y[1] = GPIO_PinInGet(gpioPortC, 1);
	gpio_data_y[2] = GPIO_PinInGet(gpioPortC, 2);
	gpio_data_y[3] = GPIO_PinInGet(gpioPortD, 6);
	gpio_data_y[4] = GPIO_PinInGet(gpioPortD, 7);
	gpio_data_y[5] = GPIO_PinInGet(gpioPortD, 8);
	gpio_data_y[6] = GPIO_PinInGet(gpioPortD, 13);
	gpio_data_y[7] = GPIO_PinInGet(gpioPortD, 14);
}
void GPIO_ODD_IRQHandler(void)
{
  /* clear flag for PC9 interrupt */
  GPIO_IntClear(0xAAAA);
  readGpioX();
  readGpioY();
}

void GPIO_EVEN_IRQHandler(void)
{
  /* clear flag for PC9 interrupt */
  GPIO_IntClear(0x5555);
  readGpioX();
  readGpioY();
}

void gpioEnableISR(void)
{
	/* Enable GPIO_ODD interrupt vector in NVIC */
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}
