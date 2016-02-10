/*
 * pwm.h
 *
 *  Created on: Feb 5, 2016
 *      Author: trungnguyen
 */
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"
#include "pwm.h"
#include "em_gpio.h"
#include <stdint.h>

//#define TOP_VAL_PWM 23280        // sets PWM frequency to 50Hz (1MHz timer clock); DETERMINE: PERIOD of PWM
#define PWM_FREQ 49
volatile uint32_t pwmWidthY, pwmWidthX;
//==========================================
/*TIMER 0 - PWM generator 1*/
void TIMER0_IRQHandler(void) {
  TIMER_IntClear(TIMER0, TIMER_IF_OF); // Clear interrupt source
  /* Do something here */
  TIMER_CompareBufSet(TIMER0, 1, pwmWidthY);       // Set new compare value, change to new duty cycle;
}

void pwmEnableISRY(void) {
	 TIMER_IntEnable(TIMER0, TIMER_IF_OF);        // Enable Timer0 overflow interrupt
	 NVIC_EnableIRQ(TIMER0_IRQn);                 // Enable Timer0 interrupt vector in NVIC
}

void setWidthPCMY(uint32_t width){
	//TIMER_CompareBufSet(TIMER0, 1, width);
	pwmWidthY = width;
}
//===========================================
/*TIMER 3 - PWM generator 2*/
void TIMER3_IRQHandler(void) {
  TIMER_IntClear(TIMER3, TIMER_IF_OF); // Clear interrupt source
  /* Do something here */
  TIMER_CompareBufSet(TIMER3, 2, pwmWidthX);       // Set new compare value, change to new duty cycle;
}

void pwmEnableISRX(void) {
	 TIMER_IntEnable(TIMER3, TIMER_IF_OF);        // Enable Timer3 overflow interrupt
	 NVIC_EnableIRQ(TIMER3_IRQn);                 // Enable Timer3 interrupt vector in NVIC
}

void setWidthPCMX(uint32_t width){
	//TIMER_CompareBufSet(TIMER3, 2, width);
	pwmWidthX = width;
}
//============================================
/*TIMER 2 - pulse generator*/
void TIMER2_IRQHandler(void) {
  TIMER_IntClear(TIMER2, TIMER_IF_OF); // Clear interrupt source
  /* Do something here */
  TIMER_CompareBufSet(TIMER2, 0, 316);       // Set new compare value, change to new duty cycle;
}

void transEnableLedISR(void) {
	 TIMER_IntEnable(TIMER2, TIMER_IF_OF);        // Enable Timer2 overflow interrupt
	 NVIC_EnableIRQ(TIMER2_IRQn);                 // Enable Timer2 interrupt vector in NVIC
}
//=============================================
/*Initialization PCM generator*/
void initPCM(void)
{
	//CMU_HFRCOBandSet(cmuHFRCOBand_1MHz);    // Set HF oscillator to 1MHz and use as system source
	CMU_ClockEnable(cmuClock_GPIO, true);   // Start GPIO peripheral clock
	CMU_ClockEnable(cmuClock_TIMER0, true); // Start TIMER0 peripheral clock - servo-motor 1 Y
	CMU_ClockEnable(cmuClock_TIMER3, true); // Start TIMER3 peripheral clock - servo-motor 2 X
	// Pulse for LED
	CMU_ClockEnable(cmuClock_TIMER2, true); // Start TIMER2 peripheral clock

	/*Setting PWM OUTPUT*/
	GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0); // set LED0 (PE2) pin as push-pull output - servo-motor 1- X axis output
	GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0); // set PD2 pin as push-pull output - servo-motor 2 output- Y axis
	// Pulse output
	GPIO_PinModeSet(gpioPortA, 12, gpioModePushPull, 0);

	/*Setting Period*/
	TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_TIMER0)/PWM_FREQ);      // PWM period will be 20 ms = 50 Hz freq
	TIMER_TopSet(TIMER3, CMU_ClockFreqGet(cmuClock_TIMER0)/PWM_FREQ);      // PWM period will be 20 ms = 50 Hz freq
	// Pulse frequency
	TIMER_TopSet(TIMER2, 632);

	/*Reset counter*/
	TIMER_CounterSet(TIMER0, 0);            // Start counter at 0 (up-count mode) Y
	TIMER_CounterSet(TIMER3, 0);            // Start counter at 0 (up-count mode) X
	// Pulse frequency
	TIMER_CounterSet(TIMER2, 0);

	// Important parameters
	// Setup Timer Channel Configuration for PWM
	TIMER_InitCC_TypeDef timerCCInit = {
		    .eventCtrl  = timerEventEveryEdge,    // This value will be ignored since we aren't using input capture
		    .edge       = timerEdgeNone,          // This value will be ignored since we aren't using input capture
		    .prsSel     = timerPRSSELCh0,         // This value will be ignored since we aren't using PRS
		    .cufoa      = timerOutputActionNone,  // No action on underflow (up-count mode)
		    .cofoa      = timerOutputActionSet,   // On overflow, we want the output to go high, but in PWM mode this should happen automatically
		    .cmoa       = timerOutputActionClear, // On compare match, we want output to clear, but in PWM mode this should happen automatically
		    .mode       = timerCCModePWM,         // Set timer channel to run in PWM mode
		    .filter     = false,                  // Not using input, so don't need a filter
		    .prsInput   = false,                  // Not using PRS
		    .coist      = false,                  // Initial state for PWM is high when timer is enabled
		    .outInvert  = false,                  // non-inverted output
		};

	 // Setup Timer Configuration for PWM
		 TIMER_Init_TypeDef timerPWMInit =
		  {
		    .enable     = true,                 // start timer upon configuration
		    .debugRun   = true,                 // run timer in debug mode
		    .prescale   = timerPrescale16,       // set prescaler to /16
		    .clkSel     = timerClkSelHFPerClk,  // set clock source as HFPERCLK
		    .fallAction = timerInputActionNone, // no action from inputs
		    .riseAction = timerInputActionNone, // no action from inputs
		    .mode       = timerModeUp,          // use up-count mode
		    .dmaClrAct  = false,                // not using DMA
		    .quadModeX4 = false,                // not using Quad Dec. mode
		    .oneShot    = false,                // not using one shot mode
		    .sync       = false,                // not syncronizing timer3 with other timers
		  };

	 // Setup Timer Configuration for Pulse
		 TIMER_Init_TypeDef timerPulseInit =
		  {
			.enable     = true,                 // start timer upon configuration
			.debugRun   = true,                 // run timer in debug mode
			.prescale   = timerPrescale1,       // set prescaler to /1
			.clkSel     = timerClkSelHFPerClk,  // set clock source as HFPERCLK
			.fallAction = timerInputActionNone, // no action from inputs
			.riseAction = timerInputActionNone, // no action from inputs
			.mode       = timerModeUp,          // use up-count mode
			.dmaClrAct  = false,                // not using DMA
			.quadModeX4 = false,                // not using Quad Dec. mode
			.oneShot    = false,                // not using one shot mode
			.sync       = false,                // not syncronizing timer3 with other timers
		  };

	/*Setting for servo-motor 2 - Y axis*/
	/*Setting compared values*/
	TIMER_CompareSet(TIMER0, 1, 0);    		// Set CC1 compare value (0% duty)
	TIMER_CompareBufSet(TIMER0, 1, 0); 		// Set CC1 compare buffer value (0% duty)
	/*Setting PWM output for TIMER0 */
	TIMER_InitCC(TIMER0, 1, &timerCCInit);       // apply channel configuration to Timer0 channel 1
	TIMER0->ROUTE = (1 << 16) |(1 << 1) | (1 << 17); // connect PWM output (timer0, channel 1) to PD2, location 3. See EFM32GG990 datasheet for details.
	/*Configure TIMER0*/
	TIMER_Init(TIMER0, &timerPWMInit);           // apply PWM configuration to timer0

	/*Setting for servo-motor 1 - X axis*/
	/*Setting compared values*/
	 TIMER_CompareSet(TIMER3, 2, 0);    		// Set CC2 compare value (0% duty)
	 TIMER_CompareBufSet(TIMER3, 2, 0); 		// Set CC2 compare buffer value (0% duty)
	 /*Setting PWM output for TIMER3 */
	 TIMER_InitCC(TIMER3, 2, &timerCCInit);       // apply channel configuration to Timer3 channel 2
	 TIMER3->ROUTE = (1 << 16) |(1 << 2);         // connect PWM output (timer3, channel 2) to PE2 (LED0) (location 1). See EFM32GG990 datasheet for details.
	 /*Configure TIMER3*/
	 TIMER_Init(TIMER3, &timerPWMInit);           // apply PWM configuration to timer3

	 /*Setting for pulse generation 1kHz*/
	 /*Setting compared values*/
	 TIMER_CompareSet(TIMER2, 0, 0);    		// Set CC0 compare value (0% duty)
	 TIMER_CompareBufSet(TIMER2, 0, 0); 		// Set CC0 compare buffer value (0% duty)
	 /*Setting PWM output for TIMER2 */
	 TIMER_InitCC(TIMER2, 0, &timerCCInit);       // apply channel configuration to Timer2 channel 0
	 TIMER2->ROUTE = (1 << 16) | (1 << 0);         	// connect PWM output (timer2, channel 0) to PA12 (location 1). See EFM32GG990 datasheet for details.
	 /*Configure TIMER2*/
	 TIMER_Init(TIMER2, &timerPulseInit);           // apply PWM configuration to timer2
}
