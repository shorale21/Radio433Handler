#include "rf_handler.h"
#include "soil_packet.h"

uint8_t compute_checksum(uint32_t packet)
{
  //sets first 8 bits to zero then computes checksum over the packet
  packet &= 0x11111100; //remove checksum
  uint8_t blocks[4];
  blocks[0] = (uint8_t)((packet & 0x00000011)); //break into 4 parts of 8
  blocks[1] = (uint8_t)((packet & 0x00001100) >> 8);
  blocks[2] = (uint8_t)((packet & 0x00110000) >> 16);
  blocks[3] = (uint8_t)((packet & 0x11000000) >> 24);
  uint8_t csum = (~blocks[0]) + (~blocks[1]) + (~blocks[2]) + ~(blocks[3]); //ones complement sum
  csum = ~csum; //ones complement
  return csum;
}

bool check_checksum(uint32_t packet)
{
  //checks the checksum
  uint8_t blocks[4]; //same as method above, packet checksum should be 0 if valid
  blocks[0] = (uint8_t)((packet & 0x00000011));
  blocks[1] = (uint8_t)((packet & 0x00001100) >> 8);
  blocks[2] = (uint8_t)((packet & 0x00110000) >> 16);
  blocks[3] = (uint8_t)((packet & 0x11000000) >> 24);
  uint8_t csum = (~blocks[0]) + (~blocks[1]) + (~blocks[2]) + ~(blocks[3]);
  csum = ~csum;
  if(csum = 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void send_INIT(uint8_t ID, uint8_t battery) //send an initialization packet
{
  /*
  INIT PACKET
  Bit 30-28: 000 (type)
  Bit 27-24: Device ID
  Bit 23: 1 (reserved)
  Bit 22-16: Device Battery Level
  Bit 15: 1 (reserved)
  Bit 14-8: TBD
  Bit 7-0: checksum
  */
  uint32_t packet = 0;
  packet |= RES_B1;
  packet |= RES_B2;
  packet |= RES_B3;
  packet |= (TYPE_INIT << TYPE_POS); //set type as INIT
  packet |= (((uint32_t)ID) << ID_POS); //set ID
  packet |= (((uint32_t)battery)) << ((uint32_t)16)); //set battery
  uint8_t csum = compute_checksum(packet); //store checksum in packet
  packet |= csum;

  send_message(packet); //send

}

void send_STAT(uint8_t ID, uint8_t battery, uint8_t time)
{
  uint32_t packet = 0;
  packet |= RES_B1;
  packet |= RES_B2;
  packet |= RES_B3;
  packet |= (TYPE_STAT << TYPE_POS); //set type as STAT
  packet |= (((uint32_t)ID) << ID_POS);
  packet |= (((uint32_t)battery)) << ((uint32_t)16));
  packet |= (((uint32_t)time)) << ((uint32_t)8);
  uint8_t csum = compute_checksum(packet);
  packet |= csum;
  send_message(packet);
}

void send_DATA(uint8_t ID, uint16_t reading) //reading is a 10bit number
{
  uint32_t packet = 0;
  packet |= RES_B1;
  packet |= RES_B2;
  packet |= RES_B3;
  packet |= (TYPE_DATA << TYPE_POS); //set type as DATA
  packet |= (((uint32_t)ID) << ID_POS);
  uint8_t data = (uint8_t)(reading >> 3); //get most significant 7 bits
  packet |= (((uint32_t)data) << 16); //16->22 are MS 7 bits
  data = (uint8_t)(reading & 7); //least significant 3 bits
  packet |= (((uint32_t)data) << 12); //12->14 are LS 3 bits
  uint8_t csum = compute_checksum(packet);
  packet |= csum;
  send_message(packet); //send
}

void send_ERR(uint8_t ID, uint8_t errno)
{
  uint32_t packet = 0;
  packet |= RES_B1;
  packet |= RES_B2;
  packet |= RES_B3;
  packet |= (TYPE_ERR << TYPE_POS); //set type as ERR
  packet |= (((uint32_t)ID) << ID_POS); //set ID
  packet |= (((uint32_t)errno)) << ((uint32_t)16)); //set error number
  uint8_t csum = compute_checksum(packet); //store checksum in packet
  packet |= csum;

  send_message(packet); //send
}

