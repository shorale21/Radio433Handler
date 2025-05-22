#include "custom_clocking.h"
#include "Arduino.h"

void (*eventHandler)(); //user specified function
uint16_t timer_start = 0; //start position for the timer

ISR(TIMER1_OVF_vect) //interrupt handler
{
  TCNT1 = timer_start; //reset timer
  eventHandler(); //call user function
  TIFR0 = 0; //reset interrupt
}

bool start_clock(uint32_t time_freq, uint32_t prescaler_bin, uint32_t prescaler_num, void (*handler)(), bool mode)
{
  //mode:
  // true -> create clock with frequency time_freq Hz
  // false -> create timer that pings every time_freq seconds (UNSUPPORTED CURRENTLY)
  eventHandler = handler; //user function
  TCCR1A = 0; //reset registers
  TCCR1B = 0;
  TCCR1B |= prescaler_bin; //set the prescaler
  TIMSK1 |= B00000001; //enable interrupts
  if(mode)
  {
    timer_start = TIM1_MAX - ((CLK_IO_SPEED / prescaler_num) / (time_freq * 2)); //calculate start point
  }
  return true;
}