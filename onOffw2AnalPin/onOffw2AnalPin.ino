/*onOff with Anal pin, used as digital input by writing value to LED's
 Uses: digitalRead, digitalWrite, INPUT_PULLUP
 */

const int ledPin =  12;
const int ledPinn =  13;  // LED output on digital pin 13

const int sensorPin = 14;   // connect wire/switch to analog pin 0 (14)  
const int sensorPinn = 15;  // ground it somehow to turn ON the LED 

void setup()
{  
  pinMode(sensorPin,INPUT_PULLUP);  // set the first 2 Anal pins input pullup HIGH
  pinMode(sensorPinn,INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);  // enable output on led pin
  pinMode(ledPinn, OUTPUT);
}

void loop()
{  //  isOff is 0 or 1, could use boolean, byte, or int ?
  bool isOff = digitalRead(sensorPin); // read the pin, HIGH (ungrounded) keeps LED off
  digitalWrite(ledPin, !isOff);       // invert so that HIGH --> off, LOW --> on
  // do same with A1
  bool isOfff = digitalRead(sensorPinn); // LOW (grounded) turns it on
  digitalWrite(ledPinn, !isOfff);       // invert so that HIGH is off, LOW is on
  delay(20);
}
