#ifndef CC_CLOCK
#define CC_CLOCK
#include <stdio.h>
//functions
extern bool start_clock(uint32_t time_freq, uint32_t prescaler_bin, uint32_t prescaler_num, void (*handler)(), bool mode);

//values
//prescalar settings for the TIM register
#define PRESCALE_1 B00000001
#define PRESCALE_8 B00000010
#define PRESCALE_64 B00000011
#define PRESCALE_256 B00000100
#define PRESCALE_1024 B00000101

//prescale values for calculation
#define PRESC_NUM_1 1
#define PRESC_NUM_8 8
#define PRESC_NUM_64 64
#define PRESC_NUM_256 256
#define PRESC_NUM_1024 1024

#define CLK_IO_SPEED 16000000 //CLK_IO speed for calculation
#define TIM1_MAX 65535 //maximum value held in the TIM register

#endif