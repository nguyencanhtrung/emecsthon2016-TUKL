/*
 * pwm.h
 *
 *  Created on: Feb 5, 2016
 *      Author: trungnguyen
 */

#ifndef PWM_H_
#define PWM_H_
#include <stdint.h>
//#include "em_timer.h"

extern volatile uint32_t pwmWidthX, pwmWidthY;

void initPCM(void);

void setWidthPCMY(uint32_t width);   // control servo motor 1 angle

void setWidthPCMX(uint32_t width);   // control servo motor 1 angle

void pwmEnableISRY(void);

void pwmEnableISRX(void);

void transEnableLedISR(void);

#endif /* PWM_H_ */
