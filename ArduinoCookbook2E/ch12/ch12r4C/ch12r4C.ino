#include <Time.h>

const int  potPin = 0;     // pot to determine direction and speed
const int  buttonPin = 2;  // button enables time adjustment

unsigned long  prevtime;   // when the clock was last displayed

void setup()
{
  digitalWrite(buttonPin, HIGH);  // enable internal pull-up resistors
  setTime(12,0,0,1,1,11); // start with the time set to noon Jan 1 2011
  Serial.begin(9600);
}

void loop()
{
  prevtime = now();   // note the time
  while( prevtime == now() )    // stay in this loop till the second changes
  {
      // check if the set button pressed while waiting for second to roll over
     if(checkSetTime())
         prevtime = now();   //  time changed so reset start time
  }
  digitalClockDisplay();
}

// functions checks to see if the time should be adjusted
// returns true if time was changed
boolean checkSetTime()
{
int value;  // a value read from the pot
int step;   // the number of seconds to move (backwards if negative)
boolean isTimeAdjusted = false;  // set to true if the time is adjusted

  while(digitalRead(buttonPin)== LOW)
  {
    // here while button is pressed
     value = analogRead(potPin);  // read the pot value
     step = map(value, 0,1023, 10, -10);  // map value to the desired range
     if( step != 0)
     {
        adjustTime(step);
        isTimeAdjusted = true; // to tell the user that the time has changed
        digitalClockDisplay(); // update clock
        delay(100);
     }
  }
  return isTimeAdjusted;
}
      

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits){
  // utility function for clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
            
    
