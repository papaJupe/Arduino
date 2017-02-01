
const int inputPin = 2;        // the number of the input pin
const int ledPin = 13;         // the number of the output pin
const int debounceDelay = 10;  // milliseconds to wait until stable
int count;   // add this variable to store the number of presses

boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  previousState = digitalRead(pin);          // store switch state
  for (int counter=0; counter < debounceDelay; counter++)
  {
      delay(1);                  // wait for 1 millisecond
      state = digitalRead(pin);  // read the pin
      if (state != previousState)
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
  Serial.begin(9600); // add this to the setup function
}

void loop()
{
  if (debounce(inputPin))
  {
    digitalWrite(ledPin, HIGH);
    count++;        // increment count
    Serial.println(count);  // display the count on the Serial Monitor
  }
}
      
    
