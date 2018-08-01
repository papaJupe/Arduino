/*
input analog voltage, from pot or sonar, map to useful values, activate toner using beep
rate and frequency to indicate distance (inversely), when target distance reached, stop
tone and
 turn on a light; set adjacent analog pins (gnd, 5v, signal) to 0 and 5 v to power pot 
 or US device
 mod A worked fine but not optimal -- long delays make loop execute very slowly when AR big;
 mod B uses timed event method to eliminate loop delays, much more complex logic, 
 and tone quality suffers from beat frequency I assume
 */

// preprocessor directives
#define potPin  A3  // analog pin to read the pot / device voltage, A1-2 can power device
#define litePin 13  // digital pin to light LED when target distance reached

// these are local vars in loop, not global, shown here for clarity
//int ARval;       //  variable reads the raw value from the analog pin 
//int beepDura;    // duration of tone() on and off in mSec
//int toneFreq;   // frequency of tone() in hz
//boolean beepOn;  // true if we're doing a timed beep event

void setup()  
{ 
  pinMode(potPin, INPUT);   // put pot ends/device to 5v and grnd, signal voltage to A3
  pinMode(A2,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(litePin,OUTPUT);  // signal lite comes on when target distance reached
  digitalWrite(A2,HIGH);
  digitalWrite(A1,LOW);
  digitalWrite(litePin,LOW);  // init LED to off

  Serial.begin(9600);   // need for printout of values, watch loop execution rate
} 

void loop() 
{ 
  static byte counter = 20;   // static sets this once, counter enables sane printout
  static boolean beepON = false;  // set once; will be set true when we start a new beep event
  static long beepStart = 0;  // times for beep event, set when new one starts
  static long beepStop = 0;   // want to keep value, not to reset each loop

  int ARval = analogRead(potPin);   // reads the volts from the potentiometer/US device 
  // 0-5000 mv. input translates to 0 to 1023, .205 ticks/mV, 
  // US device V output linearly proportional to distance ~150 mv/ft
  // thus 1 ft ~= 150*.205 = 31 ticks; and 20 ft = 3000 mV * .205 = 615 ticks
  // we'll use these values to set rate and freq. of a beeping tone
  // we want fastest rate to be 4 beeps/sec, i.e. on/off duration each 125 msec;
  // we want slowest rate to be 1/2 beep/ sec, ie on/off duration each 500 msec;
  // slowest freq 400 hz, fastest 4000 hz; so we map tick range to those 2 ranges

  // map function uses integers, but does not constrain to a range
  int beepDura = map(ARval,31,615,125,500); // postive slope so closer --> smaller intervals
  int toneFreq = map(ARval,31,615,4000,400); // negative slope so closer --> higher freq
  // so we constrain here
  beepDura = constrain(beepDura,125,500);
  toneFreq = constrain(toneFreq,400,4000);

  if (ARval >= 31)  // > 1 ft range, we use timed event to beep pin 8
  {
    digitalWrite(litePin,LOW);  // lite ON if < target distance, OFF if >
    // if beepON false we should init another beep event, set time params
    if(!beepON)
    {
      beepON = true;
      beepStart = millis();
      beepStop = beepStart + (beepDura *2); // time to stop this event

    }  
    else //  beep event is on so we do it until time is expired
    { 
      if(millis() < beepStart + beepDura)
        tone(8,toneFreq);  // tone on/off equal amounts
      else
        noTone(8);
      // if time is expired, we reset, start new event next loop w/ new params 
      if(millis() > beepStop)
        beepON= false;
    } // end else

  }
  else  // we're < 1 ft distance, turn off beep and turn on LED
  { 
    noTone(8);
    // lite the lite
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
} 





