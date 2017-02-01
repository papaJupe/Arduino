/*
 * Pullup sketch
 * a switch connected to pin 2 lights the LED on pin 13
 */

const int ledPin = 13;          // output pin for the LED
const int inputPin = 2;         // input pin for the switch

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin,HIGH);  // turn on internal pull-up on the inputPin
}

void loop(){
  int val = digitalRead(inputPin);  // read input value
  if (val == HIGH)                  // check if the input is HIGH
  {
    digitalWrite(ledPin, HIGH);     // turn LED ON
  }
  else
  {
    digitalWrite(ledPin, LOW);      // turn LED OFF
  }
}
      
    
