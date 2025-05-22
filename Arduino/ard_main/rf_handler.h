#ifndef RF_HANDLER
#define RF_HANDLER

#include <stdio.h>

extern void setup_pins(int outpin, int inpin); //setup the reading and writing pins
extern bool begin(uint16_t baud); //setup the data transmission clocking 
extern void send_message(uint32_t message); //send a message
extern uint8_t check_rx(); //check whether a message was recieved
extern uint32_t get_message(); //read message if there is one

#define MSG_BIT (((uint32_t)1) << 31) //MSB of the message, to check, all messages begin with 1

#endif