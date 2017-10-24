#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int ledPin = 13;  // LED connected to digital pin 13
double AcZ_accum = 0.0;
double AcZ_initvalue = 0.0;
const double maxg = 2.0;
const double gScale = 32768.0 / maxg;


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  //set digital pin 13 to OUTPUT
}
void loop()
{
   
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3F);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
  AcZ=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcZ_initvalue = AcZ; 
  
  for (int a = 0; a < 10; a++)
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
//    Serial.println(AcZ);  // Prints AcZ // can be enabled to read AcZ data


    const double acclZ = static_cast<double>(AcZ);

    AcZ_accum = AcZ_accum + acclZ - AcZ_initvalue;
//    Serial.println(AcZ_accum);  // can be enabled to read accumulative change of AcZ data
   delay(100);

  }

    Serial.println(AcZ_accum / gScale); //  Can be enabled to read change in gforces for Z direction
  
  if ((AcZ_accum / gScale) < -1.0 | (AcZ_accum / gScale) > 1)
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  // toggle state
   AcZ_accum = 0;
  }
  else
  {
    AcZ_accum = 0;
  }
}
