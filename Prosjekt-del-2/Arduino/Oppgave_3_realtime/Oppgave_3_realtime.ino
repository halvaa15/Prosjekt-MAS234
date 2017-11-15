#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int ledPin = 13;  // LED connected to digital pin 13
const float maxg = 2.0;
const float gScale = 32768.0 / maxg;
float values[10];
float diff = 0.0;
float totDiff = 0.0;
float accZ = 0.0;

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
  for (int i = 0; i < 10; i++)
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3F);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,2,true);  // request a total of 14 registers
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    accZ = static_cast<float>(AcZ);

    values[9] = (accZ + gScale);
    values[i] = (accZ + gScale);

    if (i == 0)
    {
      diff = (values[0]-values[9]);
    }
    else
    {
      diff = (values[i]-values[i-1]);
    }

    totDiff = totDiff + diff;
    
    Serial.println(totDiff / gScale); //  Can be enabled to read change in gforces for Z direction
    
    if ((abs(totDiff / gScale) >= 1.0) )
    {
      digitalWrite(ledPin, !digitalRead(ledPin));  // toggle state
      totDiff = 0.0;
      diff = 0.0;
      Serial.println("OVERLOAD");
      break;
    }
    delay(100);
  }
  totDiff = 0.0;
}
