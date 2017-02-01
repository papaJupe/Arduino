/*
 * Debounce sketch
 * a switch connected to pin 2 lights the LED on pin 13
 * debounce logic prevents misreading of the switch state
 */

const int inputPin = 2;        // the number of the input pin
const int ledPin = 13;         // the number of the output pin
const int debounceDelay = 10;  // milliseconds to wait until stable

// debounce returns the state when the switch is stable
boolean debounce(int pin)
{
  boolean state;
  boolean previousState;

  previousState = digitalRead(pin);          // store switch state
  for(int counter=0; counter < debounceDelay; counter++)
  {
      delay(1);                  // wait for 1 millisecond
      state = digitalRead(pin);  // read the pin
      if( state != previousState)
      {   
         counter = 0; // reset the counter if the state changes      
         previousState = state;  // and save the current state
      }
  }
  // here when the switch state has been stable longer than the debounce period
  return state;
}

void setup()
{
  pinMode(inputPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  int state = debounce(inputPin);
  digitalWrite(ledPin, state);
}
