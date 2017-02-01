/*
   AdjustClockTime sketch
   buttons on pins 2 and 3 adjust the time
 */

#include <Time.h>

const int  btnForward = 2;  // button to move time forward
const int  btnBack = 3;     // button to move time back

unsigned long  prevtime;   // when the clock was last displayed

void setup()
{
  digitalWrite(btnForward, HIGH);  // enable internal pull-up resistors
  digitalWrite(btnBack, HIGH);
  setTime(12,0,0,1,1,11); // start with the time set to noon Jan 1 2011
  Serial.begin(9600);
  Serial.println("ready");
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
int step;   // the number of seconds to move (backwards if negative 
boolean isTimeAdjusted = false;  // set to true if the time is adjusted
  step = 1;   // ready to step forwards
  while(digitalRead(btnForward)== LOW)
  {
     adjustTime(step);
     isTimeAdjusted = true; // to tell the user that the time has changed
     step = step + 1; // next step will be bigger
     digitalClockDisplay(); // update clock   
     delay(100); 
  }
  step = -1;   // negative numbers step backwards
  while(digitalRead(btnBack)== LOW)
  {
     adjustTime(step);
     isTimeAdjusted = true; // to tell the user that the time has changed
     step = step - 1; // next step will be a bigger negative number
     digitalClockDisplay(); // update clock   
     delay(100); 
  }
  return isTimeAdjusted;  // tell the user if the time was adjusted
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
    
