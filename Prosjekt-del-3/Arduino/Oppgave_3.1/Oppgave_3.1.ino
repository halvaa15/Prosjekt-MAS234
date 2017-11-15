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

const int ledPin = 13;  //Creates a variable ledPin which is connected to the LED on the teensy

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

// -------------------------------------------------------------
void setup(void)
{
  pinMode(ledPin, OUTPUT);    //Sets the LED on the teensy as a output
  digitalWrite(ledPin, HIGH); //Sets the LED high when the program starts

  struct CAN_filter_t defaultMask;

  defaultMask.flags.remote = 0;
  defaultMask.flags.extended = 0;
  defaultMask.id = 0;
  
  Can0.begin(250000,defaultMask, 1, 1);  
  Can1.begin();

  pinMode(28, OUTPUT);   
  pinMode(35, OUTPUT);

  digitalWrite(28, LOW);
  digitalWrite(35, LOW);
}

// -------------------------------------------------------------
void loop(void)
{ 
  CAN_message_t inMsg;
  while (Can0.available()) 
  {
    Can0.read(inMsg);
   
    if (inMsg.id == 0x21) //Checks if Can0 recives a message on the ID 0x21
    {
      digitalWrite (ledPin, !digitalRead(ledPin));  //If the if-statement is true, the LED on the teensy toggels
    }
     if (((inMsg.buf[7] &= (1)) == (1)) && (inMsg.id == (0x22))) 
    {
      digitalWrite(ledPin, HIGH);   // The LED is written high
      Serial.println("Høy");

    }
    // If the least significant bit in the first byte AND 1 equals 1, and the message ID is 0x22
    if (((inMsg.buf[7] &= (1)) == (0)) && (inMsg.id == (0x22)))
    {
      digitalWrite(ledPin, LOW);    // The LED is written low
      Serial.println("Lav");
    }
  }
}
