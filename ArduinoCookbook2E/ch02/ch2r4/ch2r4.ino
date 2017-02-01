/*
 array sketch
 an array of switches controls an array of LEDs
 see Chapter 5 for more on using switches
 see Chapter 7 for information on LEDs
 */

int inputPins[] = {2,3,4,5};  // create an array of pins for switch inputs

int ledPins[] = {10,11,12,13};  // create array of output pins for LEDs

void setup()
{
  for(int index = 0; index < 4; index++)
  {
    pinMode(ledPins[index], OUTPUT);         // declare LED as output
    pinMode(inputPins[index], INPUT);        // declare pushbutton as input
    digitalWrite(inputPins[index],HIGH);     // enable pull-up resistors
                                             //(see Recipe 5.2)
  }
}

void loop(){
  for(int index = 0; index < 4; index++)
  {
    int val = digitalRead(inputPins[index]);  // read input value
    if (val == LOW)                       // check if the switch is pressed
    {
      digitalWrite(ledPins[index], HIGH); // turn LED on if switch is pressed
    }
    else
    {
      digitalWrite(ledPins[index], LOW);      // turn LED off
    }
  }
}
      
    
