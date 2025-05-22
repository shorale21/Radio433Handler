/*
 * extihandle.c
 *
 *  Created on: Apr 28, 2025
 *      Author: alexa
 */


#include "main.h"
#include "extihandle.h"

uint8_t test = 1;

uint8_t getTest()
{
	return test;
}

void initialize()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN_Msk;
	GPIOC->MODER &= ~GPIO_MODER_MODE2;
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2;
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD2;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD2_1;

	SYSCFG->EXTICR[0] &= ~(0xf << 8);
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC;
	EXTI->IMR1 |= EXTI_IMR1_IM2; //interrupt masking
	EXTI->RTSR1 |= EXTI_RTSR1_RT2; //rising trigger enable

	NVIC->ISER[0] |= (1 << (EXTI2_IRQn & 0x1f));
	__enable_irq();
}

