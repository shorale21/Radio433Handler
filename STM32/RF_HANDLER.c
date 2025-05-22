
#include "main.h"
#include "RF_HANDLER.h"
#include "USARTHandler.h"

uint32_t out_msg = 0; // packets are required to be 32 bits and must start with a 1
// MSB being 1 indicates message being sent status
// messages are sent MSB first
uint8_t out_pos = 31; //position inside the packet to send
uint32_t rf_rx = 0; //receiving register
uint32_t rx_count = 31; //position inside the rx to add
uint8_t pulse = 0; //set when a pulse is received
uint8_t receiving = 0; //set when a message is receiving


void init_output_port() //output port configured to PC0
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~(OUT_MODE_MSK);
	GPIOC->MODER |= OUT_MODE;
	GPIOC->OTYPER &= ~(OUT_MSK);
	GPIOC->OTYPER |= OUT_OTYPE;
	GPIOC->OSPEEDR &= ~(OUT_MODE_MSK);
	GPIOC->OSPEEDR |= OUT_OSPEED;
	GPIOC->PUPDR &= ~(OUT_MODE_MSK);
	GPIOC->PUPDR |= OUT_PUPD;
}

void init_input_port() //input port (Unecessary for external interrupt usage)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~(IN_MODE_MSK);
	//GPIOC->MODER |= IN_MODE;
	GPIOC->OTYPER &= ~(IN_MSK);
	GPIOC->OTYPER |= IN_OTYPE;
	GPIOC->OSPEEDR &= ~(IN_MODE_MSK);
	GPIOC->OSPEEDR |= IN_OSPEED;
	GPIOC->PUPDR &= ~(IN_MODE_MSK);
	GPIOC->PUPDR |= IN_PUPD;
}

void start_clock(uint32_t freq, uint32_t duty)
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; //enable TIM2
	uint32_t arr = MAINCLK / freq; //calculate max arr value (cycles to count)
	TIM2->ARR = arr - 1; //set arr
	TIM2->CCR1 = (uint32_t)((arr) * ((float)duty / 100)); //set compare register
	TIM2->CCER |= TIM_CCER_CC1E; //enable first capture/compare register
	TIM2->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE); //enable interrupts for ccr1

	NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1f)); //handle interrupts
	__enable_irq(); //interrupt handling
	TIM2->CR1 |= TIM_CR1_CEN; //enable tim2 counting
	//GPIOC->ODR |= OUT_MSK;
	return;
}

void send_message(uint32_t message)
{
	if(out_msg == 0)
	{
		out_msg = message;
		out_pos = 31;
	}
}

uint8_t check_rx()
{
	if(rx_count == 0)
	{
		return 1;
	}
	return 0;
}

uint32_t get_message()
{
	uint32_t temp = rf_rx;
	rf_rx = 0;
	rx_count = 31;
	return temp;
}

void TIM2_IRQHandler() //called every rising and lowering edge
{
	//UART_print("ir",2);
	if(TIM2->SR & ARR_BIT) //rising edge
	{
		TIM2->SR &= ~(ARR_BIT);
		//detect input on rising edge
		GPIOC->ODR &= 0;
		if(receiving == 1)
		{
			if(pulse == 1) //if detected a pulse, set 1
			{
				pulse = 0;
				rf_rx |= 1 << rx_count;
			}
			if(rx_count == 0) //if finished receiving message
			{
				receiving = 0;
			}
			else
			{
				rx_count--;
			}
		}

	}
	else if(TIM2->SR & CCR1_BIT) //falling edge
	{
		TIM2->SR &= ~(CCR1_BIT);
		//change output on falling edge
		GPIOC->ODR &= 0;
		if((out_msg & MSG_BIT) > 0) //if bit 31 is 1
		{
			GPIOC->ODR |= ((out_msg & (1 << out_pos)) >> out_pos);
			if(out_pos == 0)
			{
				out_pos = 31; //reset values after message is sent
				out_msg = 0;
			}
			else
			{
				out_pos--;
			}
		}
	}
	return;
}

void EXTI2_IRQHandler()
{
	//GPIOC->ODR |= 1;
	EXTI->PR1 |= (EXTI_PR1_PIF2_Msk);
	pulse = 1;
	receiving = 1;
	//UART_printChar('r');
}
