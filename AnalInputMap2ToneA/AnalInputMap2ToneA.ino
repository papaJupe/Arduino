/*
 input analog voltage, from pot or US, map to useful values, activate toner using beep
 rate and frequency to indicate distance (inversely); when target distance reached, stop tone and
 turn on a light; set adjacent analog pins (gnd, 5v, signal) to 0 and 5 v to power pot or US device
 use of delays slows loop markedly but not a problem here; later versions improve this a lot

 Uses: analogRead, device power from adjacent pins, tone, map, print w/ String concat,
 print with interval, constrain variable
 */

// preprocessor directives
#define potPin A2  // analog pin to read the potentiometer voltage, nearby can power device
#define gndPin A0  // 14 Uno, 54 Mega
#define powPin A4
#define litePin 13  // digital pin lights LED when target distance reached

   // used only in loop, so could be local vars instead; defined here for clarity
int ARval;    //  variable reads the raw value from the analog pin 
int beepDura;    // duration of tone() on and off in mSec
int toneFreq;   // frequency of tone() in hz

void setup()  
{ 
  pinMode(potPin, INPUT);   // put pot ends to 5v and grnd, variable voltage to potPin
  pinMode(gndPin,OUTPUT);
  pinMode(powPin,OUTPUT);
  pinMode(litePin,OUTPUT);
  digitalWrite(powPin,HIGH);
  digitalWrite(gndPin,LOW);
  digitalWrite(litePin,LOW);  // init LED to off

  Serial.begin(9600);   // need for printout of values
} 

void loop() 
{ 
    static byte counter = 20;   // static sets this once, for sane printout rate

  ARval = analogRead(potPin);   // reads input volts from the potentiometer/US device 
  // 0-5000 mv. input translates to 0 to 1023 ticks, .205 ticks/mV, 
  // US device V output linearly proportional to distance ~150 mv/ft
  // thus 1 ft ~= 150 * .205 = 31 ticks; and 20 ft = 3000 mV * .205 = 615 ticks
  // we'll use these values to set rate and freq. of a beeping tone
  // we want fastest rate to be 4 beeps/sec, i.e. on/off duration each 125 msec;
  // we want slowest rate to be 1 beep/ sec, i.e. on/off duration each 500 msec;
  // slowest freq 400 hz, fastest 4000 hz; so we map tick range to those 2 ranges

  // map uses integers, but does not constrain to a range ...
  beepDura = map(ARval,31,615,125,500); // positive slope so closer --> smaller intervals
  toneFreq = map(ARval,31,615,4000,400); // negative slope so closer --> higher freq
  // so we constrain here
  beepDura = constrain(beepDura,125,500);
  toneFreq = constrain(toneFreq,400,4000);

  if (ARval >= 31)  // > 1 ft range, we beep on pin 8
  {
    digitalWrite(litePin,LOW);  // LED is off until we get < 1 ft
    tone(8,toneFreq);  // on/off equal amounts, device on pin 8
    delay(beepDura);
    noTone(8);
    delay(beepDura);
  }
  else  // we're < 1 ft distance
  { 
    noTone(8);
    // lite the lite
    digitalWrite(litePin,HIGH);
  }

  //
  //  // map fx I/O uses integers, so we could convert input value in ticks to distance in ft.
 int val = map(ARval, 31, 615, 1, 20);  // and print to Ser. Mon.
 Serial.println(val); // in ft.

  if(counter==20)   // print every 20 loops
    { String myValue = String(ARval,DEC);
      Serial.println(String ("raw pin AR is " + myValue + " ticks"));
      counter = 0;
    }
    counter++;
    
  delay(40);                 
} 




