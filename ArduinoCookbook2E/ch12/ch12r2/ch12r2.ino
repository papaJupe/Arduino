/*
  millisDuration sketch
  returns the number of milliseconds that a button has been pressed
 */

const int switchPin = 2;                   // the number of the input pin

long startTime; // the value returned from millis when the switch is pressed
long duration;  // variable to store the duration

void setup()
{
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH); // turn on pull-up resistor
  Serial.begin(9600);
}


void loop()
{
  if(digitalRead(switchPin) == LOW)
  {
     // here if the switch is pressed
     startTime = millis();
     while(digitalRead(switchPin) == LOW)
        ; // wait while the switch is still pressed
     long duration = millis() - startTime;
     Serial.println(duration);
  }
}
      
    
