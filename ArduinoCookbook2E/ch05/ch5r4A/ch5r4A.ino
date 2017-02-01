/*
 * SwitchTimeMultiple sketch
 * Prints how long more than one switch has been pressed
 */

const int switchAPin = 2;               // the pin for switch A
const int switchBPin = 3;               // the pin for switch B

// functions with references must be explicitly declared
unsigned long switchTime(int pin, boolean &state, unsigned long  &startTime);

void setup()
{
  pinMode(switchAPin, INPUT);
  digitalWrite(switchAPin, HIGH); // turn on pull-up resistors
  pinMode(switchBPin, INPUT);
  digitalWrite(switchBPin, HIGH); // turn on pull-up resistors
  Serial.begin(9600);
}

void loop()
{
unsigned long time;

  Serial.print("switch A time =");
  time = switchATime();
  Serial.print(time);

  Serial.print(", switch B time =");
  time = switchBTime();
  Serial.println(time);
  delay(1000);
}

unsigned long switchTime(int pin, boolean &state, unsigned long  &startTime)
{
  if (digitalRead(pin) != state) // check to see if the switch
                                // has changed state
  {
    state = ! state;       //yes,  invert the state
    startTime = millis();  // store the time
  }
  if (state == LOW)
    return millis() - startTime;   // return the time in milliseconds
  else
    return 0; // return 0 if the switch is not pushed (in the HIGH state);
}


long switchATime()
{
  // these variables are static  - see text for an explanation
  static unsigned long startTime = 0;   // the time the switch 
                                        // state change was first detected
  static boolean state;                 // the current state of the switch
  return switchTime(switchAPin, state, startTime);
}

long switchBTime()
{
  // these variables are static  - see text for an explanation
  static unsigned long startTime = 0;   // the time the switch state change
                                        // was first detected
  static boolean state;                 // the current state of the switch
  return switchTime(switchBPin, state, startTime);
}
      
    
