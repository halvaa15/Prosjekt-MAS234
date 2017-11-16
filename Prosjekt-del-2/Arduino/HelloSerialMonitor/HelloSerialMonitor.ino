/* Serial Monitor Example, Teensyduino Tutorial #3
   http://www.pjrc.com/teensy/tutorial3.html

   After uploading this to your board, use Serial Monitor
   to view the message.  When Serial is selected from the
   Tools > USB Type menu, the correct serial port must be
   selected from the Tools > Serial Port AFTER Teensy is
   running this code.  Teensy only becomes a serial device
   while this code is running!  For non-Serial types,
   the Serial port is emulated, so no port needs to be
   selected.

   This example code is in the public domain.
*/


void setup()   {                
  Serial.begin(9600); // start serial with baud rate of 9600
}

void loop()                     
{
  Serial.println("Hello serial monitor");
  delay(1000);
}

