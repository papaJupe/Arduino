/*
simple test to read single register from FRC accelerometer using I2C
Just uses Wire lib & its basic commands
 */

#include "Wire.h"

// default 7 bit I2C address (Spark_ & Ada_) is 0x53 (83) , not for FRC bd

// int16_t ax, ay, az;  // 16 bit, 2 byte, i.e. Ardu's basic int type for accel reads

const byte devAdd = 0x1D;  //  = (29 = 1st 7 bits of 0x3A(58) device address claimed by FRC

byte deviceID;  // in register 0x00, should be 0xE5 (229) per AD datasheet; seems OK

#define LED_PIN 13 // built-in Uno lite

bool blinkState = false;
//------------------------------------------

void setup() 
{
  pinMode(A2,OUTPUT); // for convenience I use anal pins as voltage sources for device
  pinMode(A3,OUTPUT);   // A4-5 are SDA and SCL
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v
  
  // join I2C bus (I2Cdev library doesn't do this automatically; optional arg = this dev addr, if slave)
  Wire.begin();

  // initialize serial communication
  // (38400 works as well at 8MHz as it does at 16MHz, but change to fit your project)
  Serial.begin(38400);

  // verify connection -- see if dev ID=0xE5 (229) or what
  Serial.println("Getting device ID...");
   Wire.beginTransmission(devAdd);  // at the address 
   Wire.write(0x00);            // we want to read this register
   Wire.endTransmission();  // send the pointer bytes
   
  if(Wire.requestFrom(devAdd,1))    // request 1 byte from slave 7 bit addr devAdd
      { //Wire.requestFrom(address, quantity, stop) stop: def true = stop req. if value returned
     delay (5);
     Serial.println("request valid to address " + String(devAdd));
      } 
  else
     {  Serial.println("no reply from addr " + String(devAdd) + " -- stopping");
     delay(500); // or it quits before printing stuff
     exit(1);  // stops execution
     }   // end else--  if we didn't stop => read byte(s) sent back
   while(Wire.available())    // slave may send less (more?) than requested
     {
     byte b = Wire.read();    // receive a byte as byte, .print shows its dec #
     Serial.println("got back " + String(b));  // shows the byte as #
     }  

  // configure LED for output, should be default for digi pinn
  pinMode(LED_PIN, OUTPUT);
   
}  // end setup

void loop() 
{
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
  delay(800);
}

