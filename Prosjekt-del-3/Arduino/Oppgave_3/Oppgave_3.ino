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

#include<Wire.h>
  const int MPU_addr=0x68;  // I2C address of the MPU-6050
  int16_t AcX,AcY,AcZ;

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

const int ledPin = 13;

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
  digitalWrite(ledPin, HIGH);

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

    //Code to make the IMU work
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

}


// -------------------------------------------------------------
void loop(void)
{ 
  CAN_message_t inMsg;
  
  if (ledMetro.check())   //inMsg.id == 32
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    Serial.print("AcX = "); Serial.print(AcX);
    Serial.print(" | AcY = "); Serial.print(AcY);
    Serial.print(" | AcZ = "); Serial.println(AcZ);
  }
  
  
  while (Can0.available()) 
  {
    Can0.read(inMsg);
   
    if (inMsg.id == 0x21)
    {
      digitalWrite (ledPin, !digitalRead(ledPin));
      Serial.println("Lyset toggles");
    }
    
    if (((inMsg.buf[7] &= (1)) == (1)) && (inMsg.id == (0x22)))
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("Lyset settes høy");
    }
    
    if (((inMsg.buf[7] &= (1)) == (0)) && (inMsg.id == (0x22)))
    {
      digitalWrite(ledPin, LOW);
      Serial.println("Lyset settes lavt");
    }
    
    
  }
}
