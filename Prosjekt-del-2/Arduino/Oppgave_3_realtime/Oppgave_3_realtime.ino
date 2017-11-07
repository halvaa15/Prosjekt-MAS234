#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int ledPin = 13;  // LED connected to digital pin 13
const double maxg = 2.0;
const double gScale = 32768.0 / maxg;
double verdier[10];
double differanse = 0.0;
double totDiff = 0.0;
double accZ = 0.0;

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

    accZ = static_cast<double>(AcZ);

    verdier[9-i] = abs(accZ);

    if (i == 9)
    {
      differanse = abs(verdier[9]-verdier[0]);
    }
    else
    {
      differanse = abs(verdier[8-i]-verdier[9-i]);
    }

    totDiff = totDiff + differanse;
    
    Serial.println(totDiff / gScale); //  Can be enabled to read change in gforces for Z direction
    
    if ((totDiff / gScale) > 1.0)
    {
      digitalWrite(ledPin, !digitalRead(ledPin));  // toggle state
      totDiff = 0.0;
      Serial.print("hei");
      break;
    delay(100);
    }
  }
  totDiff = 0.0;
}
