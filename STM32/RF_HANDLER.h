/*
 * RF_HANDLER.h
 *
 *  Created on: Apr 26, 2025
 *      Author: alexa
 */

#ifndef SRC_RF_HANDLER_H_
#define SRC_RF_HANDLER_H_

#include "main.h"

//defininitions to generate a square wave of frequency and duty cycle

extern void init_output_port(void);
extern void start_clock(uint32_t freq, uint32_t duty);
extern void TIM2_IQRHandler(void);
extern void send_message(uint32_t message);
extern uint8_t check_rx();
extern uint32_t get_message();
extern void init_input_port();

//define output pin as PC0

#define OUT_MSK GPIO_ODR_OD0_Msk
#define OUT_MODE_MSK GPIO_MODER_MODE0_Msk

#define OUT_MODE GPIO_MODER_MODE0_0 //output
#define OUT_OTYPE 0 //push pull
#define OUT_OSPEED 0 //low speed
#define OUT_PUPD GPIO_PUPDR_PUPD0_1 //use pull down resistor

// define input pin as PC1
#define IN_MSK GPIO_ODR_OD1_Msk
#define IN_MODE_MSK GPIO_MODER_MODE1_Msk
#define IN_MODE GPIO_MODER_MODE1 //input
#define IN_OTYPE 0 //push pull
#define IN_OSPEED 0
#define IN_PUPD GPIO_PUPDR_PUPD0_1 //pull down resistor

#define IN_PORT 1

//constants
#define MAINCLK 24000000// 4MHz
#define ARR_BIT TIM_SR_UIF
#define CCR1_BIT TIM_SR_CC1IF
#define MSG_BIT (1 << 31)



#endif /* SRC_RF_HANDLER_H_ */
