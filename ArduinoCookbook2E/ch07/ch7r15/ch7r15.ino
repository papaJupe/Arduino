/*
 * AnalogMeter sketch
 * Drives an analog meter through an Arduino PWM pin
 * The meter level is controlled by a variable resistor on an analog input pin
 */

const int analogInPin = 0; // Analog input pin connected to the variable resistor
const int analogMeterPin = 9; // Analog output pin connecting to the meter

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup()
{
  // nothing in setup
}

void loop()
{
  sensorValue = analogRead(analogInPin);            // read the analog in value
  outputValue = map(sensorValue, 0, 1023, 0, 255);  // map to the range of the
                                                    // analog out
  analogWrite(analogMeterPin, outputValue);         // write the analog out value
}
      
    
