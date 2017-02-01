/*
 * I2C_7segment
 * Uses I2C port to drive a bar graph
 * Turns on a series of LEDs proportional to a value of an analog sensor.
 * see Recipe 7.5
 */

#include <Wire.h>

//address for PCF8574 with pins connected as shown in Figure 13-12
const int address = 0x38;  
const int NbrLEDs = 8;

const int analogInPin = 0; // Analog input pin connected
                           // to the variable resistor

int sensorValue = 0;       // value read from the sensor
int ledLevel = 0;          // sensor value converted into LED 'bars'
int ledBits = 0;           //  bits for each LED will be set to 1 to turn on LED

void setup()
{
  Wire.begin(); // set up Arduino I2C support
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(analogInPin);             // read the analog in value
  ledLevel = map(sensorValue, 0, 1023, 0, NbrLEDs);  // map to number of LEDs
  for (int led = 0; led < NbrLEDs; led++)
  {
    if (led < ledLevel ) {
      bitWrite(ledBits,led, HIGH);    // turn on LED if less than the level
    }
    else {
      bitWrite(ledBits,led, LOW);    // turn off LED if higher than the level
    }
    // send the value to I2C
    Wire.beginTransmission(address);
    Wire.write(ledBits);
    Wire.endTransmission();
  }
  delay(100);
}