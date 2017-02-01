/*
 * Repeat
 * blinks while a condition is true
 */

const int ledPin    = 13; // digital pin the LED is connected to
const int sensorPin = 0; // analog input 0

void setup()
{ 
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT); // enable LED pin as output
}

void loop()
{
  while(analogRead(sensorPin) > 100)
  {
    blink();    // call a function to turn an LED on and off
    Serial.print(".");
  }
  Serial.println(analogRead(sensorPin)); // this is not executed until after
                                        // the while loop finishes!!!
}  

void blink()
{
   digitalWrite(ledPin, HIGH);
   delay(100); 
   digitalWrite(ledPin, LOW);
   delay(100);
}
