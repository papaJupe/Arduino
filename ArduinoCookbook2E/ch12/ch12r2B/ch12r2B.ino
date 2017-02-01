#include <TimedAction.h>

//initialize a TimedAction class to change LED state every second.
TimedAction timedAction = TimedAction(NO_PREDELAY,1000,blink);


const int ledPin =  13;      // the number of the LED pin
boolean ledState = LOW;


void setup()
{
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,ledState);
}

void loop()
{
  timedAction.check();
}

void blink()
{
  if (ledState == LOW)
    ledState = HIGH;
  else
    ledState = LOW;

  digitalWrite(ledPin,ledState);
}   
    
