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

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

const int ledPin = 13;

Metro ledMetro = Metro(1000);

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
  delay(1000);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);   //Setter ledPin høy når programmet starter

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
  if (ledMetro.check())
  {
    Serial.println("Ja");
  }
  
  CAN_message_t inMsg;
  while (Can0.available()) 
  {
    Can0.read(inMsg);
   
    if (inMsg.id == 33)
    {
      digitalWrite (ledPin, !digitalRead(ledPin));
      Serial.println("Lyset toggles");
    }
    
    if (((inMsg.buf[7] &= (1)) == (1)) && (inMsg.id == (34)))
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("Lyset settes høy");
    }
    
    if (((inMsg.buf[7] &= (1)) == (0)) && (inMsg.id == (34)))
    {
      digitalWrite(ledPin, LOW);
      Serial.println("Lyset settes lavt");
    }
    
    
  }
}
