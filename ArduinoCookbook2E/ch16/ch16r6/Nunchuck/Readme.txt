Readme.txt file for Arduino Nunchuck Library

This library provides an Arduino interface to the wii nunchuck.

The library supports the WiiChuck adapter on standard boards (with I2C pins on
Digital 18 and 19) and  Mega boards (with I2C pins on 20 and 21).
for info on the WiiChuck adapter see: (http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/)
Any board can be used by wiring the nunchuck to the correct I2C and power pins.

The library was written for Arduino 1.0 but also work with earlier releases.

Here is an example sketch:

include <Wire.h>
#include "Nunchuck.h"

void setup()
{
    Serial.begin(9600);
    nunchuckSetPowerpins(); // power nunchuck using WiiiChuck adapter
    nunchuckInit();         // send the initialization handshake
    nunchuckRead();         // this gets fresh data ready for the next call
    delay(50);
}

void loop()
{
  nunchuckRead();
  Serial.print("H,");       // header
  for(int i=0; i < wii_ItemCount; i++) // print all values as CSV data to serial port
  {
     Serial.print(nunchuckGetValue(i), DEC);
     Serial.write(',');
  }
  Serial.println();
  delay(20); // the time in milliseconds between redraws
}

The functions available in the library include:

 Use pins adjacent to I2C pins as power & ground for Nunchuck
   void nunchuckSetPowerpins();
 
 Initialize the I2C interface for the nunchuck
   void nunchuckInit();
 
 Request data from the nunchuck
   void nunchuckRequest();

 Receive data back from the nunchuck, returns true if read successful, else false
   bool nunchuckRead();
 
 Encode data to format that most wiimote drivers except
  char nunchuckDecode (uint8_t x);
  
 Return the value for the given item. Items are enumerates as:
  Joysticks: wii_joyX, wii_joyY
  Accelerometers: wii_accelX, wii_accelY, wii_accelZ
  Buttons: wii_btnZ, wii_btnC
   int nunchuckGetValue(int item);
  
 For example, after calling nunchuckRead, to get the value of the y joystick:
    int yPos = nunchuckGetValue(wii_yoyY); 
	
 To test if button C is pressed:	
    if(nunchuckGetValue(wii_btnC) == true)
	   // do something if pressed