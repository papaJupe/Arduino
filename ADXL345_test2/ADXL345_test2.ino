/*
ADXL345_test2 (to read single register from accelerometer)  amended to scan 0-127 
for address of active device, then get its ID from reg 0x00
Uses: I2C, Wire lib and basic commands to R/W
*/

#include "Wire.h"

// default 7 bit I2C address (Spark_ & Ada_) is 0x53 (83) -- 0x1D (29) for FRC board

// int16_t ax, ay, az;  // 16 bit, 2 byte, i.e. Ardu's basic int type for accel reads

byte devAdd = 0x00;  // starting point for scan

byte deviceID;  // should be 0xE5 (229) per AD datasheet; held in register 0x00

#define LED_PIN 13 // (Arduino LED on pin 13)

bool blinkState = false;
//------------------------------------------

void setup() 
{
  pinMode(A2,OUTPUT); // for convenience I use anal pins as power for device
  pinMode(A3,OUTPUT);   // A4-5 are SDA and SCL
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v
  
  delay (1000); // time for device to power up
 
  // join I2C bus; optional arg = this dev addr, if slave)
  Wire.begin();

  // initialize serial communication
  // (38400 works as well at 8MHz as it does at 16MHz, but change to fit your project)
  Serial.begin(38400);

  // verify connection -- to see if val=0xE5; print what val you get from Wire.read()
  Serial.println("Getting device ID...");
  for (byte i=0x0000; i<0x0080; i++) // try to find valid addr 0 to 127
  {
    Wire.beginTransmission(devAdd + i);  // at the address 
    Wire.write(0x00);            // in request, we want to read this register
    Wire.endTransmission();      // send the pointer bytes

    if(Wire.requestFrom(devAdd+i,1))    // request 1 byte from slave 7 bit (<=127) addr devAdd
    {    //Wire.requestFrom(address, quantity, stop) arg 3, def. to true, stop req. if value returned
      delay (5);
      Serial.print("request valid to address "), Serial.println(devAdd+i);
      while(Wire.available())    // slave may send less (more?) than requested
      {  // may just read buffer's real content, or not -- need to test
        byte b = Wire.read();    // receive a byte as byte, .print shows its dec #
        Serial.println("got back " + String(b));  // shows the byte as #
      }  // alternative req., simpler, avoiding wire.available
      
//   { //Wire.requestFrom(address, quantity, stop) stop: def true = stop req. if value returned
//      delay (5);
//      Serial.println("request valid to address " + String(devAdd));
//      byte b = Wire.read();    // receive a byte as byte, .print shows its dec #
//      Serial.println("got back " + String(b));  // .print shows the byte as #
//    }

    } // end if good response
    else
     {  
      Serial.println("no reply from addr " + String(devAdd+i));
      delay(50);
     }   // end else, req. failed

  }  // end loop testing addr #s
  
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
