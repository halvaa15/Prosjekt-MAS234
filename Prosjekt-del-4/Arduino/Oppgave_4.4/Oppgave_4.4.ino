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

#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyX,GyY,GyZ;

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
  //Declares a variable inMsg which contains the data of the recived CAN-message
  CAN_message_t inMsg;
  CAN_message_t outMsg;
  
  while (Can0.available()) 
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43);  // starting with register 0x43 (GYRO_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,6,true);  // request a total of 6 registers

    Can0.read(inMsg); //Reads the message which is recived in Can0

    if (inMsg.id == 0x234) //Checks if Can0 recives a message on the ID 0x234
    {
      int8_t gyro [6];
      gyro[0]=Wire.read();  // 0x43 (GYRO_XOUT_H) 
      gyro[1]=Wire.read();  // 0x44 (GYRO_XOUT_L)
      gyro[2]=Wire.read();  // 0x45 (GYRO_YOUT_H) 
      gyro[3]=Wire.read();  // 0x46 (GYRO_YOUT_L)
      gyro[4]=Wire.read();  // 0x47 (GYRO_ZOUT_H)
      gyro[5]=Wire.read();  // 0x48 (GYRO_ZOUT_L)

      outMsg.id = 0x111;  // Gives the message ID 0x111
      outMsg.len = 6U;    // The lenght of the message is 6 unsigned int
      memcpy (outMsg.buf, gyro , 6U);
      Can0.write(outMsg);
    }
  }
}
