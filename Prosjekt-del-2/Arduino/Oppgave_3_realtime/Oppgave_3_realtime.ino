#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcZ;
int ledPin = 13;  // LED connected to digital pin 13
const float maxg = 2.0; // maximum g the IMU will measure
const float gScale = 32768.0 / maxg;  // scaling frow raw data to gforce
float values[10]; // array to store acceleration in Z direction data the last 1s
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
  for (int i = 0; i < 10; i++)  // loop to gather and process data each 100ms over 1s
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3F);  // starting with register 0x3B (ACCEL_ZOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    accZ = static_cast<float>(AcZ); // converts int16_t datatype to float

    values[9] = (accZ + gScale);  // 
    values[i] = (accZ + gScale);

    if (i == 0) // 
    {
      diff = (values[0]-values[9]);
    }
    else
    {
      diff = (values[i]-values[i-1]);
    }

    totDiff = totDiff + diff; // varaible to store total change of accelaration i Z direction
    
    Serial.println(totDiff / gScale); //  prints change in gforce to terminal
    
    if ((abs(totDiff / gScale) >= 1.0) )  // if change in gforce exceeds 1g, block will be triggered
    {
      digitalWrite(ledPin, !digitalRead(ledPin));  // toggles led
      totDiff = 0.0;  // totDiff value will be reset before starting a new measuring cycle
      Serial.println("OVERLOAD"); // prints OVERLOAD message to terminal
      break;  // ends for loop to start a new measuring cycle
    }
    delay(100); // delay to read values every 100ms
  }
  totDiff = 0.0;   // if measuring time exceeds 1s(cycle ends), totDiff value will be reset before new measuring cycle starts.
}
