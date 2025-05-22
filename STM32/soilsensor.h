/*
 * soilsensor.h
 *
 *  Created on: May 19, 2025
 *      Author: alexa
 */

#ifndef SRC_SOILSENSOR_H_
#define SRC_SOILSENSOR_H_

#include "main.h"

extern uint8_t verify_packet(uint32_t packet);
extern uint8_t compute_checksum(uint32_t packet);

#define STAT_REQ ((uint32_t)2576980377) //packet from controller
#define DATA_REQ ((uint32_t)2863311530)

#define TYPE_INIT ((uint32_t)0) //various packet types
#define TYPE_STAT ((uint32_t)1)
#define TYPE_DATA ((uint32_t)2)
#define TYPE_ERR ((uint32_t)3)

#define RES_B1 (((uint32_t)1) << 31) //bits that are always true, for error checking
#define RES_B2 (((uint32_t)1) << 23)
#define RES_B3 (((uint32_t)1) << 15)

#define TYPE_MSK (((uint32_t)7) << 28) //place of type
#define ID_MSK (((uint32_t)15) << 24) //place of ID

#endif /* SRC_SOILSENSOR_H_ */
