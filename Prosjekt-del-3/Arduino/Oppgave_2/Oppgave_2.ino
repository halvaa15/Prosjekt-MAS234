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

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}


// -------------------------------------------------------------
void setup(void)
{
  //defaultMask to apply to all mailboxes
  struct CAN_filter_t defaultMask;

  // Default mask is allow everything
  defaultMask.flags.remote = 0;
  defaultMask.flags.extended = 0;
  defaultMask.id = 0;

  //Defining which Can-port we are using, also the baudrate, which filter, Tx = 1 and Rx = 1 to enable alternate pins.
  Can0.begin(250000,defaultMask, 1, 1);  
  Can1.begin();

  //Using the alternate Can0 pins and set the as outputs
  pinMode(28, OUTPUT);
  pinMode(35, OUTPUT);

  //Setting the value of the pins low
  digitalWrite(28, LOW);
  digitalWrite(35, LOW);

  msg.ext = 0;
  msg.id = 0x100;
  msg.len = 8;
  msg.buf[0] = 10;
  msg.buf[1] = 20;
  msg.buf[2] = 0;
  msg.buf[3] = 100;
  msg.buf[4] = 128;
  msg.buf[5] = 64;
  msg.buf[6] = 32;
  msg.buf[7] = 16;
}


// -------------------------------------------------------------
void loop(void)
{ 
  //Declares a variable inMsg which contains the data of the recived CAN-message
  CAN_message_t inMsg;
  while (Can0.available()) 
  {
    
    Can0.read(inMsg); //Reads the message which is recived in Can0
 
    Serial.print("CAN bus 0: "); hexDump(8, inMsg.buf);  //Writes out the informaton in the message
    Serial.println(inMsg.buf[2]);   //Writes out the information in the 3rd byte
    Serial.println(inMsg.id);   //Writes out the message ID

    Can0.write(inMsg);    //Writes a message and sends it through Can0, the message contains the same data as inMsg
  }
}
