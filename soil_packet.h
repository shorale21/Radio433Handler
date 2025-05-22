#ifndef SOIL_PACKET
#define SOIL_PACKET
#include <stdio.h>
extern uint8_t compute_checksum(uint32_t packet); //checksum for error
extern bool check_checksum(uint32_t packet); //check the checksum of an incoming packet
extern void send_INIT(uint8_t ID, uint8_t battery); //sending various packet types
extern void send_STAT(uint8_t ID, uint8_t battery, uint8_t time);
extern void send_DATA(uint8_t ID, uint16_t reading); //send data packet
extern void send_ERR(uint8_t ID, uint8_t errno);

#define TYPE_MSK (((uint32_t)7) << 28) //place of type
#define ID_MSK (((uint32_t)15) << 24) //place of ID

#define ID_POS 24 //position of ID in packet (same for all types)
#define TYPE_POS 28 //position of TYPE in packet (same for all types)

#define ERR_UNKNOWN 2 //unknown packet error

#define TYPE_INIT ((uint32_t)0) //various packet types
#define TYPE_STAT ((uint32_t)1)
#define TYPE_DATA ((uint32_t)2)
#define TYPE_ERR ((uint32_t)3)

#define RES_B1 (((uint32_t)1) << 31) //bits that are always true, for error checking
#define RES_B2 (((uint32_t)1) << 23)
#define RES_B3 (((uint32_t)1) << 15)

// controller messages: controller will request specific packets to be broadcasted
#define STAT_REQ ((uint32_t)2576980377) //packet from controller 
#define DATA_REQ ((uint32_t)2863311530)

#endif

/*
Message Format

1 _ _ _ _ _ _ _ 1 _ _ _ _ _ _ _ 1 _ _ _ _ _ _ _ _ _ _ _ _ _ _ _


Bit 31: 	1 (reserved)
Bit 30-28:  Message type
		-> possible types:
			000 - initialization packet
			001 - status packet
			010 - sensor data packet
			011 - error packet
			
INIT PACKET
Bit 30-28: 000
Bit 27-24: Device ID
Bit 23: 1
Bit 22-16: Device Battery Level
Bit 15: 1
Bit 14-8: TBD
Bit 7-0: checksum

STATUS PACKET
Bit 30-28: 001
Bit 27-24: Device ID
Bit 23: 1
Bit 22-16: Device Battery Level (7 bit)
Bit 15: 1
Bit 14-8: Time to next data collect
Bit 7-0: checksum

SENSOR DATA PACKET
Bit 30-28: 010
Bit 27-24: Device ID
Bit 23: 1
Bit 22-16: Sensor data (MS 7 bits)
Bit 15: 1
Bit 14-12: Sensor Data (LS 3 bits)
Bit 11-8: TBD
Bit 7-0: checksum

ERROR DATA PACKET
Bit 30-28: 010
Bit 27-24: Device ID
Bit 23: 1
Bit 22-16: error type
Bit 15: 1
Bit 14-8: TBD
Bit 7-0: checksum
*/