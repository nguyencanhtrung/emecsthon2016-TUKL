/*
 * gpio.h
 *
 *  Created on: Feb 5, 2016
 *      Author: trungnguyen
 */

#ifndef GPIO_H_
#define GPIO_H_
#include <stdbool.h>

extern bool gpio_data_x[8], gpio_data_y[8];
extern int error;
void gpioInit(void);

void gpioEnableISR(void);

#endif /* GPIO_H_ */
