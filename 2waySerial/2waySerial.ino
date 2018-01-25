/* 2waySerial -- to AT program BT module from Ardu IDE

  Setup for MEGA:
   gnd for level shift to pin 13+1

  Serial2 to BLE module, pins: TX=16, RX=17; or Ser Soft on Uno
*/


// Emulate Serial1 on pins 6/7 if hardware serial (like Mega) not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#define LED_PIN 13
int c = 0;

//SoftwareSerial SofSer(6, 7); // RX, TX

void setup() 
{
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {

  while (Serial2.available()) 
  {
    Serial.print((char)Serial2.read());
  }

  while (Serial.available()) 
  {
    Serial2.print((char)Serial.read());
  }
  
  delay(10);
}   // end loop
