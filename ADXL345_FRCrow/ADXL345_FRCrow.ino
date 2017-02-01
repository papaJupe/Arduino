// Arduino demo of I2C device and ADXL345 accelerometer class (libs)
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-10-07 - initial release
//     2016-05-02 edited for FRC board by AM
/* ============================================
 I2Cdev device library code is placed under the MIT license
 Copyright (c) 2011 Jeff Rowberg
 ===============================================
 */

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and ADXL345 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
// ADXL345 lib has dozens of advanced chip functions controlling data range/freq.
// tap, free-fall sensitivity, sleep, storing data on chip...

#include "I2Cdev.h"
#include "ADXL345.h"

// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 ((83) 7 bit default for SparkFun & Ada_ board)
// ALT high = 0x1D ((29) seems to be default for FRC board; FRC ALT pin ?= D0?)
// addr 0x3A in AD's sheet is 8 bit version, not used in software
ADXL345 accel = ADXL345(0x1D); // (29)

int16_t ax, ay, az;  // 16 bit, 2 byte, i.e. Ardu's basic int type

#define LED_PIN 13 // (Arduino is 13, Teensy is 6)
bool blinkState = false;

void setup() 
{
  pinMode(A2,OUTPUT); // for convenience I use 2 anal pins as voltage sources for device
  pinMode(A3,OUTPUT); 
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v
  
  // join I2C bus (I2Cdev library doesn't do this automatically; optional arg = this dev addr, if slave)
  Wire.begin();

  // initialize serial communication
  // (38400 works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(9600);  // this should clear buffer I/O
  // Serial.flush();  // clear garbage from buffer (out buffer only)
  // initialize device -- fx in .cpp, uses param sent by ADXL345 constructor
  Serial.println("Initializing I2C devices...");
  accel.initialize();

  // verify connection -- doesn't really; calls getDeviceID() which is 'true' even if value returned
  // matches 0 (which I tried in .cpp and is not a real devID); doesn't test -ACK bit; real devID is 0xE5
  Serial.println("Testing device connections...");
  Serial.println(accel.testConnection() ? "ADXL345 connection made" : "ADXL345 connection failed");

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  delay(5);
  accel.setRate(ADXL345_RATE_100);  // enum-->var value of 10 = 100 hz meas rate
  delay(5);
  byte rate = accel.getRate();  // default is 100 hz = ret. value of 10 here; 9 means 50 hz etc.
  Serial.println("meas. rate is  " + String(rate) + "\n -- v. adxl345.h for equiv. hz");
}

void loop() 
{
  // read raw accel measurements from device; would need to calib. to G's, mG's, m/sec/sec, degrees etc
  accel.getAcceleration(&ax, &ay, &az);  // I think: in C, passing by ref. means fx writes val back to var

  // display tab-separated accel x/y/z values
  Serial.print("accel:\t");
  Serial.print(ax); 
  Serial.print("\t");
  Serial.print(ay); 
  Serial.print("\t");
  Serial.println(az);

  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
  delay(800);
}
