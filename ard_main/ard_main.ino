#include "custom_clocking.h"
#include "rf_handler.h"
#include "soil_packet.h"
#include <Arduino.h>

uint16_t desired_freq = 20; // hertz
uint16_t prescl = 8;
uint8_t DEVICE_ID = 1;

uint8_t compute_battery()
{
  float level = (float)analogRead(A1); //read in voltage, note: will be half the actual
  float volts = (level * 5) / ((float)255); //map 0 to 255 to 0 to 5
  volts *= 2.0; //get actual output
  volts += 1.0; //adjustment for experimental error
  float percent = (volts - 6.9) * (100) / (3.3) //map 6.9-9.9 to 0-100
  return ((uint8_t)percent);
}

void setup() {
  // put your setup code here, to run once:
  interrupts(); //enable interrupts
  pinMode(A1, INPUT); //battery reading pin
  Serial.begin(9600); //start Serial Terminal (COMMENT OUT IF MOBILE)
  setup_pins(8, 2); //setup the RF handler
  begin(800); //start the radio communication
  delay(1000); //wait 1 second
  uint8_t b = compute_battery();
  send_INIT(DEVICE_ID, b); //broadcast an initialization packet for the controller
}

void loop() {
  if(check_rx() == 1) //message recieved
  {
    uint32_t = message = get_message();
    if(message == STAT_REQ) //if requesting the device status
    {
      send_STAT(DEVICE_ID, compute_battery(), 0);
    }
    else if(message == DATA_REQ) //if requesting a sensor reading
    {
      send_DATA(DEVICE_ID, 0);
    }
    else //send error oterhwise
    {
      send_ERR(DEVICE_ID, ERR_UNKNOWN);
    }
  }
}
