/*
   LED bar graph - decay version
*/

const int ledPins[] = { 2, 3, 4, 5, 6, 7};
const int NbrLEDs = sizeof(ledPins) / sizeof(ledPins[0]);
const int analogInPin = 0; // Analog input pin connected to variable resistor
const int decay = 10;      // increasing this reduces decay rate of storedValue

int sensorValue = 0;       // value read from the sensor
int storedValue = 0;       // the stored (decaying) sensor value
int ledLevel = 0;          // value converted into LED 'bars'

void setup() {
  for (int led = 0; led < NbrLEDs; led++)
  {
    pinMode(ledPins[led], OUTPUT);  // make all the LED pins outputs
  }
}


void loop() {
  sensorValue = analogRead(analogInPin);        // read the analog in value
  storedValue = 
    max(sensorValue, storedValue);  // only use sensor value if higher
    
  ledLevel = map(storedValue, 0, 1023, 0, NbrLEDs); // map to number of LEDs
  for (int led = 0; led < NbrLEDs; led++)
  {
    if (led < ledLevel ) {
      digitalWrite(ledPins[led], HIGH);  // turn on pins less than the level
    }
    else {
      digitalWrite(ledPins[led], LOW);   // turn off pins higher than the level
    }
  }
  storedValue = storedValue - decay;     // decay the value
  delay(10);                             // wait 10 ms before next loop
}
      
    
