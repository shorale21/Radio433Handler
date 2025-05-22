/*
 * soilsensor.c
 *
 *  Created on: May 19, 2025
 *      Author: alexa
 */


#include "soilsensor.h"
#include "main.h"


uint8_t compute_checksum(uint32_t packet)
{
	uint8_t blocks[4]; //break into 4 blocks of 8 bits
	blocks[0] = (uint8_t)((packet & 0x00000011));
	blocks[1] = (uint8_t)((packet & 0x00001100) >> 8);
	blocks[2] = (uint8_t)((packet & 0x00110000) >> 16);
	blocks[3] = (uint8_t)((packet & 0x11000000) >> 24);
	uint8_t csum = (~blocks[0]) + (~blocks[1]) + (~blocks[2]) + ~(blocks[3]); //ones complement sum
	csum = ~csum;
	if(csum == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
uint8_t verify_packet(uint32_t packet) //must have 3 bits correct and the correct checksum
{
	if((packet & RES_B1) == 0)
	{
		return 0;
	}
	if((packet & RES_B2) == 0)
	{
		return 0;
	}
	if((packet & RES_B3) == 0)
	{
		return 0;
	}
	return compute_checksum(packet);
}
