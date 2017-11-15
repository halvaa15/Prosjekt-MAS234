// -------------------------------------------------------------
// CANtest for Teensy 3.6 dual CAN bus
// by Collin Kidder, Based on CANTest by Pawelsky (based on CANtest by teachop)
//
// Both buses are left at default 250k speed and the second bus sends frames to the first
// to do this properly you should have the two buses linked together. This sketch
// also assumes that you need to set enable pins active. Comment out if not using
// enable pins or set them to your correct pins.
//
// This sketch tests both buses as well as interrupt driven Rx and Tx. There are only
// two Tx buffers by default so sending 5 at a time forces the interrupt driven system
// to buffer the final three and send them via interrupts. All the while all Rx frames
// are internally saved to a software buffer by the interrupt handler.
//

#include <FlexCAN.h>

#include <Metro.h>
Metro ledMetro = Metro(1000);

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

// -------------------------------------------------------------
void setup(void)
{
  struct CAN_filter_t defaultMask;

  defaultMask.flags.remote = 0;
  defaultMask.flags.extended = 0;
  defaultMask.id = 0;
  
  Can0.begin(250000,defaultMask, 1, 1);  
  Can1.begin();

  //if using enable pins on a transceiver they need to be set on
  pinMode(28, OUTPUT);
  pinMode(35, OUTPUT);

  digitalWrite(28, LOW);
  digitalWrite(35, LOW);
}


// -------------------------------------------------------------
void loop(void)
{ 
  CAN_message_t outMsg;
  int8_t i = 0;
  while (ledMetro.check() == 1) 
  {
    i++;
    Serial.println(i);
    if (i%2 == 1) // Checks if i is a odd number
    {
      outMsg.id = 0x21; //Set the message ID to 0x21
      Can0.write(outMsg);
    }
     if (i%2 == 0)  // Checks if i is an even nummber
    {
      outMsg.id = 0x22; //Set the message ID to 0x21
      Can0.write(outMsg);
    }
  }
}
