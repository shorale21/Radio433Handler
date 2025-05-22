#include "rf_handler.h"
#include "custom_clocking.h"
#include <Arduino.h>

uint32_t out_msg = 0; // packets are required to be 32 bits and must start with a 1
// MSB being 1 indicates message being sent status
// messages are sent MSB first
uint8_t out_pos = 31; //position inside the packet to send
uint32_t rf_rx = 0; //receiving register (shift register)
uint32_t rx_count = 31; //position inside the rx to add
uint8_t pulse = 0; //set when a pulse is received
uint8_t receiving = 0; //set when a message is receiving
bool posedge = false;
int outpin = 0;
int inpin = 0;


void exti_handle() //handle external interrupts
{
	if(pulse == 0) //debug
	{
		Serial.println("r");
	}
	pulse = 1; //recieved a pulse
	receiving = 1; //currently recieving a message
	EIFR |= 1; //reset interrupt mask
}

void setup_pins(int outpin_, int inpin_) //inpin must be 2 or 3
{
	//Serial.println("Setting Up");
  pinMode(outpin_, OUTPUT); //set the pin for writing
  //pinMode(inpin_, INPUT);
  outpin = outpin_; //store pins
  inpin = inpin_;
	temp = (inpin_ == 2) ? 0 : ((inpin_ == 3) ? 1 : 0);
	attachInterrupt(temp, exti_handle, RISING); //attach external interrupt to handler function
}

void timer_handler() //handle the clock
{
	//message is written to the transmitter on the falling edge, and read in on the rising edge
  posedge = !posedge; //reverse edge for clock
  digitalWrite(outpin, LOW);
  if(posedge) //rising edge
  {
    if(receiving == 1)
		{
			if(pulse == 1) //if detected a pulse, set 1
			{
				pulse = 0;
				rf_rx |= ((uint32_t)1) << rx_count; //set bit in recieved message
			}
			if(rx_count == 0) //if finished receiving message
			{
				receiving = 0;
				//pulse = 0;
			}
			else
			{
				rx_count--; //decrement position
			}
		}
  }
  else //falling edge writing data
  {
    if((out_msg & MSG_BIT) > 0) //if bit 31 is 1 (if there is a message to send)
		{
			digitalWrite(outpin, ((out_msg & (((uint32_t)1) << out_pos)) >> out_pos));
			if(out_pos == 0) //if done
			{
				out_pos = 31; //reset values after message is sent
				out_msg = 0; //remove message
			}
			else
			{
				out_pos--; //decrement position
			}
		}
  }
}

bool begin(uint16_t baud) //start clocking
{
  return start_clock(baud, PRESCALE_8, PRESC_NUM_8, &timer_handler, true);
}

void send_message(uint32_t message) //send a message, set the out_msg to the desired message, which triggers the writing on the falling edge
{
	if(out_msg == 0)
	{
		out_msg = message;
		out_pos = 31;
		// Serial.print("Sending Message: ");
		// Serial.println(out_msg & MSG_BIT);
	}
}

uint8_t check_rx() //check if rx has a message
{
	if(rx_count == 0) //if message finished recieving
	{
		return 1;
	}
	return 0;
}

uint32_t get_message() //read message, rx_count is only reset once the message is read.
{
	uint32_t temp = rf_rx;
	rf_rx = 0;
	rx_count = 31;
	receiving = 0;
	return temp;
}
