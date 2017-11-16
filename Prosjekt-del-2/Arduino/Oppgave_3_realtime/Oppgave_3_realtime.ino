#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050 (no pull up resistor)
int16_t AcZ;
int ledPin = 13;  // LED connected to digital pin 13
const float maxg = 2.0; // maximum g the IMU will measure
const float gScale = 32768.0 / maxg;  // scaling from raw data to gforce
float values[10]; // array to store acceleration in Z direction data the last 1s
float diff = 0.0;
float totDiff = 0.0;
float accZ = 0.0;
int j = 0;  // variable for counting for-loops without trigger

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
  for (int i = 0; i <= 10; i++)  // loop to gather and process data each 100ms over 1s
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3F);  // starting with register 0x3F (ACCEL_ZOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    accZ = static_cast<float>(AcZ); // converts int16_t datatype to float

    if (values[9] == 0.0)
    {
    values[9] = (accZ + gScale);  // sets value[9] to be able to make first diff calculation      
    }

    values[i] = (accZ + gScale);  // write acceleration in Z direction + compensating for gravity
    j++;

    if (i == 0) // 
    {
      diff = (values[0]-values[9]); // need specific operation for start of loop
    }
    else
    {
      diff = (values[i]-values[i-1]); // diff = current value - previous value
    }

    totDiff = totDiff + diff; // varaible to store total change of accelaration i Z direction
    
    Serial.println(totDiff / gScale); //  prints change in gforce to terminal
    if ((abs(totDiff / gScale) >= 1.0) )  // if change in gforce exceeds 1g, block will be triggered
    {
      digitalWrite(ledPin, !digitalRead(ledPin));  // toggles led
      totDiff = 0.0;  // totDiff value will be reset before starting a new measuring cycle
      Serial.println("OVERLOAD"); // prints OVERLOAD message to terminal
      j = 0;
    }
    if (j >= 10)  // if loops without triggering exceeds 10 resets totDiff and loop counting
    {
      totDiff = 0.0;
      j = 0;
    }
    delay(100); // delay to read values every 100ms
    
  }
}
