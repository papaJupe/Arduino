
// i2c_scanner -- tests the standard 7-bit addresses 0-127 for valid device(s)
//
// Version 1.
//    The original author is not known
// Version 2, 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// minor mods by AM 2016

#include <Wire.h>
byte error, address;
int nDevices =0;
  
void setup()
{
  pinMode(A2,OUTPUT); // for convenience I use anal pins as voltage sources for device
  pinMode(A3,OUTPUT);   // A4-5 are SDA and SCL
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v
  Wire.begin();

  Serial.begin(38400);  // could be anything
 // while (!Serial);        // makes Leonardo wait for serial monitor
  Serial.println("\nI2C Scanner");
}

void loop()
{
  Serial.println("Scanning...");

  for(address = 0; address < 128; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmission() fx to see if
    // a device did acknowledge what it sends
    Wire.beginTransmission(address);
    // Wire.write(0x00);  // just some data to write, optional
    error = Wire.endTransmission();  // sends address w/ 0 then 1 @ end, gets ACK or not
    delay(10);
    if (error==0)  // gets this only if both addr (and data if sent) got ACK
      {
        Serial.print("I2C device found at address 0x");
        if (address<16) 
         Serial.print("0"); // 1st 15 need a 0 in front
        Serial.print(address,HEX);
        Serial.println(" !");
   
        nDevices++;
      }
    else if (error==4) 
      {
        Serial.print("Unknown error at address 0x");
        if (address<16) 
         Serial.print("0");  // 0 before hex #
        Serial.println(address,HEX);
      }
    else 
      {
        Serial.print("got error at address 0x");
        if (address<16) 
         Serial.print("0");  // 0 before hex #
        Serial.print(address,HEX); Serial.print("  ");
        Serial.println(error);
      }  
   // delay(200);  
  }   //end for
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("done -- found "+ String(nDevices) + " devices");
    delay(500);  // give time to print before ...
  exit(1); // stops loop or could keep going:
  // delay(5000);        //  to wait 5 seconds for next scan
  // nDevices = 0;
}  // end loop
