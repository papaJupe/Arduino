
/*
 input analog voltage, from pot or sonar, map to useful values, activate toner using beep
 rate and frequency to indicate distance (inversely); when target distance reached, stop tone and
 turn on a light; set adjacent analog pins (gnd, 5v, signal) to 0 and 5 v to power pot or US device
 mod A worked fine but not optimal -- long delays make loop execute very slowly when AR big;
 mod B uses timed event method to eliminate loop delays; much more complex logic, and tone quality
 suffers from beat frequency I assume
 mod C uses timedAction lib to make beeps run in their own thread so loop rate not affected; tone 
 quality is fine, and I can modulate beep rate in loop, and freq by setting it in beep().
 Uses: anal read, tone, TimedAction lib, count for sane printing, map, constrain
 */

// preprocessor directives
#define potPin  A3  // analog pin to read the pot / device voltage, A1-2 can power device
#define litePin 13  // digital pin to light LED when target distance reached

#include <TimedAction.h>

TimedAction beepONoff = TimedAction(1000,beep);  // object runs its own 'thread'

// ARval local var, shown here for clarity, others are global
//int ARval;       //  variable reads the raw value from the analog pin 
int beepDura = 1000;    // duration of tone() on and off in mSec, put into TA; reset in loop
int toneFreq = 700;   // frequency of tone() in hz, set in loop, reset in beep() fx
boolean toneOn = false;  // true if we're toning, false if quiet

void setup()  
{ 
  pinMode(potPin, INPUT);   // put pot ends/device supply to 5v and grnd, signal voltage to A_
  pinMode(A2,OUTPUT);
  pinMode(A1,OUTPUT);
  digitalWrite(A2,HIGH);
  digitalWrite(A1,LOW);

  pinMode(litePin,OUTPUT);  // signal lite comes on when target distance reached
  digitalWrite(litePin,LOW);  // init LED to off

  Serial.begin(9600);   // need for printout of values, watch loop execution rate
} 

void loop() 
{ 
  static byte counter = 20;   // static sets this once, counter enables sane printout

  int ARval = analogRead(potPin);   // reads the volts from the potentiometer/US device 
  // 0-5000 mv. input translates to 0 to 1023, .205 ticks/mV, 
  // US device V output linearly proportional to distance ~150 mv/ft
  // thus 1 ft ~= 150*.205 = 31 ticks; and 20 ft = 3000 mV * .205 = 615 ticks
  // we'll use these values to set rate and freq. of a beeping tone
  // we want fastest rate to be 5 beeps/sec, i.e. on/off duration each 100 msec;
  // we want slowest rate to be 1/2 beep/ sec, ie on/off duration each 500 msec;
  // slowest freq 400 hz, fastest 4000 hz; so we map tick range to those 2 ranges
  static int prevARval = ARval+1;  // just set 1st loop

  // map function uses integers, but does not constrain to a range
  int beepDura = map(ARval,31,615,100,500); // positive slope so closer --> smaller intervals
  // so we constrain here
  beepDura = constrain(beepDura,100,500);
  // toneFreq = constrain(toneFreq,400,4000);  // now read and applied in beep()

  if (ARval >= 31)  // > 1 ft range, we use timedAction to beep pin 8
  {
    beepONoff.enable();
    digitalWrite(litePin,LOW);  // lite OFF if > target distance from something

    if(ARval != prevARval)
    {  // send a new interval to the timedAction
      beepONoff.setInterval(beepDura);
      delay(5);
      beepONoff.check();
    }  
    else // value same so just check up on it
    { 
      beepONoff.check();
    } // end else

  }
  else  // we're < 1 ft distance, turn off beep and turn on LED
  {  
    beepONoff.disable();
    noTone(8);
   
    digitalWrite(litePin,HIGH);
  }

  //
  //  // map fx I/O uses integers; we could convert input value in ticks to distance in ft.
  //  val = map(ARval, 31, 615, 1, 20);  // and print to Ser. Mon.
  //  Serial.println(val); // in ft.

  if(counter==20)   // print every 20 loops
  {  
    String myValue = String(ARval,DEC);
    Serial.println(String ("raw pin AR is " + myValue + " ticks"));
    counter = 0;
  }
  counter++;

  delay(50);                 
}   // end loop


void beep()
{
  int ARval = analogRead(potPin);
  int toneFreq = map(ARval,31,615,3600,600);
  toneFreq = constrain(toneFreq,600,4700);
  if(toneOn) tone(8,toneFreq);
  else noTone(8);
  toneOn = !toneOn;  // timed Actions of beepDura should alternate on and off toning

}

