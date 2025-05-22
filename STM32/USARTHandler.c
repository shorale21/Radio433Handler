/*
 * USARTHandler.c
 *
 *  Created on: May 8, 2024
 *      Author: alexa
 */


#include "main.h"
#include "USARTHandler.h"

void UART_init()
{
	RCC->AHB2ENR |= IOEN;
	IOPORT->MODER &= ~(MODER_MSK);
	IOPORT->OTYPER &= ~(OTYPE_MSK);
	IOPORT->OSPEEDR &= ~OSPEED_MSK;
	IOPORT->PUPDR &= ~PUPDR_MSK;
	IOPORT->AFR[0] &= ~AF_MSK;
	IOPORT->MODER |= MODER_SET;
	IOPORT->AFR[0] |= AF_SET;

	RCC->APB1ENR1 |= UEN;

	UAPORT->CR1 &= 0;
	UAPORT->CR2 &= 0;
	UAPORT->CR3 &= 0;
	UAPORT->BRR = Brr;
	UAPORT->CR1 |= USART_CR1_TE;
	UAPORT->CR1 |= USART_CR1_RE;
	UAPORT->CR1 |= USART_CR1_UE;
}


void UART_printChar(char c)
{
	while(!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = c;
}


void UART_print(char str[], uint32_t len)
{
	for(int i = 0; i < len; i++)
	{
		UART_printChar(str[i]);
	}
	//UART_printChar('\0');
}

void UART_ESC(char code[], uint32_t len)
{
	UART_printChar(27);
	UART_print(code, len);
}

char UART_read()
{
	if(USART2->ISR & USART_ISR_RXNE)
	{
		return (char)USART2->RDR;
	}
	return NOCHAR;
}

void UART_printNum(int n)
{
	if(n == 0)
	{
		UART_printChar('0');
		return;
	}
	int stop = n;
	int i = 0;
	char num[100];
	char nTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	while(stop != 0)
	{
		num[i] = nTable[stop % 10];
		stop = stop / 10;
		i++;
	}
	for(int j = i - 1; j >= 0; j--)
	{
		UART_printChar(num[j]);
	}
	return;
}

void UART_printDec(int n, int place) //place is the location of the decimal point from the right
{
	if(n == 0)
		{
			UART_printChar('0');
			return;
		}
		int stop = n;
		int i = 0;
		char num[100];
		char nTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
		while(stop != 0)
		{
			num[i] = nTable[stop % 10];
			stop = stop / 10;
			i++;
		}
		for(int j = i - 1; j >= 0; j--)
		{
			if(j == place)
			{
				UART_printChar('.');
			}
			UART_printChar(num[j]);
		}
		return;
}
