const int ledPin =  A1;        // LED connects to digital pin 13
const int sensorPin = 10;    // connect wire/switch to analog pin 5, ground it to turn ON the LED

void setup()
{
  pinMode(A3, OUTPUT);
  pinMode(ledPin, OUTPUT);  // enable output on the led pin
  pinMode(sensorPin, INPUT_PULLUP);
  digitalWrite(A3, LOW); // ground for LED on A1
}

void loop()
{ //  isOff is 0 or 1, can use boolean here too
  int isOff = digitalRead(sensorPin); // read the pin, HIGH (ungrounded) keeps LED off, L (grounded) turns it on
  digitalWrite(ledPin, !isOff);       // invert so that HIGH is off, LOW is on

  delay(50);
}
