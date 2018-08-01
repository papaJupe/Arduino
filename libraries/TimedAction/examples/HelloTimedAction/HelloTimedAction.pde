#include <TimedAction.h>

//pin / state variables
const byte ledPin = 13;
boolean ledState = false;

// must define fx before using it to init object
void blink() {
  ledState ? ledState = false : ledState = true;
  digitalWrite(ledPin, ledState);
}

//this initializes a TimedAction class that will change the state of an LED every second.
TimedAction timedAction = TimedAction(1000, blink);
 
void setup() {

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void loop() {
  timedAction.check();
  //do something else
}

