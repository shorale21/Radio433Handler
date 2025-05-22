/*
 * USARTHandler.h
 *
 *  Created on: May 8, 2024
 *      Author: alexa
 */

#include "main.h"

// header file for the usart handler

//PA2 is transmit
//PA3 is to recieve

// ===================== Functions =======================

extern void UART_init();
extern void UART_print(char str[], uint32_t len);
extern void UART_printChar(char c);
extern void UART_ESC(char code[], uint32_t len);
extern char UART_read();
extern void UART_printNum(int n);
extern void UART_printDec(int n, int place);

// ===================== PORT CONFIG =======================

#define IOPORT GPIOA
#define UAPORT USART2
#define IOEN RCC_AHB2ENR_GPIOAEN
#define UEN RCC_APB1ENR1_USART2EN
#define MODE 2
#define AF 7
#define MODER_MSK (GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk)
#define OTYPE_MSK (GPIO_OTYPER_OT2_Msk | GPIO_OTYPER_OT3_Msk)
#define OSPEED_MSK (GPIO_OSPEEDR_OSPEED2_Msk | GPIO_OSPEEDR_OSPEED3_Msk)
#define PUPDR_MSK (GPIO_PUPDR_PUPD2_Msk | GPIO_PUPDR_PUPD3_Msk)
#define MODER_SET ((MODE << GPIO_MODER_MODE2_Pos) | (MODE << GPIO_MODER_MODE3_Pos))
#define AF_MSK (GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk)
#define AF_SET ((AF << GPIO_AFRL_AFSEL2_Pos) | (AF << GPIO_AFRL_AFSEL3_Pos))

#define Brr 2500 //9.6 kbps
#define NOCHAR 0



