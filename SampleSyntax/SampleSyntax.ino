// preprocessor directives, before setup or wherever you expect them to operate
/*  multiline comment
    declare things at top before setup so all blocks can use them
*/ e.g.
#include <systemlibrary.h> or "userlibrary.h" from users documents/ardu/libraries folder

const [type] [NAME] = [value];  // safer than #define to set a constant, gcc checks type

#define [something] [its value]     // needs no = or ;

    from wikibooks C prog -- should also work in Ardu C --
    Another feature of the #define command is that it can take arguments, 
    making it rather useful as a pseudo-function creator. Consider the following:
    
    #define ABSOLUTE_VALUE( x ) ( ((x) < 0) ? -(x) : (x) )
       use it later
    int x = -1;
    while( ABSOLUTE_VALUE( x ) )  // runs while body until x becomes 0? 
    ...
    
from cookbook p 88, after showing use of word(hi,low) to make 2 byte word from
a high and low byte, also extract them from a word w/ highByte(), lowByte()

#define makelong(hi,lo) ((hi) << 16 | (lo))  -- makes 32 bit big word from
2 16 bit words

    It is generally a good idea to use extra parentheses when using complex macros. 
    Notice that in the above example, the variable "x" is always within its own set of 
    parentheses. This way it will be evaluated as itself, before being compared to 
    0 or multiplied by -1. Also, the entire macro is surrounded by parentheses, to 
    prevent it from being contaminated by other code. [Even then they advise against
    using Macros like this; instead use inline function ? whatever that is]
----------------------------------------------
binary # represented in code as Bxxxxxx, e.g. B0001100 for 12

byte(x) converts number x to byte format
-------------------------
void setup()  // put your setup code here, to run once:
{
 Serial.begin(speed);  // initialize serial port to send/receive at 9600 baud
 pinMode(pinName, INPUT|OUTPUT|INPUT_PULLUP);  // OK for any pin
 pinMode(A0, INPUT);  // A0 same as 14
 pinMode(15, INPUT_PULLUP);   // same as A1, makes high until pulled low
 digitalWrite(15, HIGH); // old way to pull-up, will be high unless switch pulls it low
}

void loop() 
{
  // put main code here, runs repeatedly
   exit(1); // exit(int) undocumented C fx, stops loop, sends int to OS
   not sure how OS interprets the param
}
-------------------------

// array specify

[var type] [name] [#of elements]; or various ways
  int myInts[6];   // initialize to size without specifying elements
  int myPins[] = {2, 4, 8, 3, 6}; //myPins[2] evaluates to 8
  int mySensVals[6] = {2, 4, -8, 3, 2};  // element 6 defaults to 0 I think
  // storing one string, it's an array of chars, type is char
  char message[6] = "hello";  //size=one more than chars in it, ends with NULL; 
                       // defaults to null value if no init or deleted?
      // array storing many strings (arrays), type is char *
  char * letters[] = {".-", "-...", "-.-.", "-.."}; // array of arrays, char* a pointer, 
        // i.e. letters is array of char pointers? or does *name signify name = pointer? Y
     //spaces optional, char*name is same    
     
   // can you empty an existing array without using loop on each element, ? by re-init, no  
     // out[0]=0;  // zeros (nulls) 1st element only and 2nd?, leaves others intact 
       // from prev. loop 
       // this, re-init does not always clear the array:
     int out[8] = {0}; // new declar ? set all 8 slots to 0's, sizeof() = 16 bytes, each
                       // int = 2 bytes; but may not work as intended
             //this should reset all to 0, but inelegant  
    int out[8] = {0,0,0,0,0,0,0,0}; 
    //or  void * memset ( void * ptr, int value, size_t num );< proto in C already?
   memset(in, 0, 8);
   memset(out, 0, 8);  // clears properly; e.g. ReadSerialIntC
   
   int out[5] = {2};   // new declar puts 2 in [0] or [5]?, 0 in others(or not?), each int slot=2 byte
                          // so sizeof(out) is 10, whether the slots are filled or null
                          // can get # of elements w/ (sizeof(myInts)/sizeof(int)) if myInts holds ints
    
    // char out[8];  // re-declar (w/o init) puts null in all slots? -- no, leaves what's there 
    // char out[8] = {'0'}; // new declar puts ch 0 in [0], null others, sizeof()=8, since
    each ch slot=1 byte
    char out[8] = {0};   // new declar puts #0 (null), in all slots, sizeof() = 8 bytes, 
       // since each ch = 1 byte
           // sizeof(out) is 8, whether the slots are filled or null

use enum constants to 'label' array elements
--------------------------------------------------
// another undocumented C feature, sort of like find(something) by index
enum {zilch, alpo, burka, carly, darpa}; 
// when used in fx, useEm(burka), the index of burka in the array is sent as param
// example Cookbook p. 431

//loops and conditionals IF WHILE DO
---------------------------
  if (brightness > 253)  // must be true or false
    {                    // brackets optional for one line block
       brightness = brightness - 1;  // count down after reaching 254
    }  //end if
  else if(brightness < 2)
    {
      brightness = brightness + 2; // count up after dropping down to 1
    }  // end else if
    
  for (byte i=0; i < 4; i++)  //why use int when it fits in byte?
    {              // brackets needed for more than one line
                // write the brightness value to the LEDs
         analogWrite(pinOut[i], brightness);
         if(some condx you want to end for loop) break; // exits
     } // end for
     
     // can send PWM also to A0 et al? if you pinMode(A0-?others, OUTPUT) and aW(A0,0-255) 
     // or for some boards w/ 10 bit DAC on A0: aW(A0,1022) -- may be steady dc for latter
     
   while (Serial.available()) // do stuff while at least one character is available
  {    
    char ch = Serial.read();    // automatically reads each char and deletes from buffer
    Serial.read();    // just this works to clear buffer if you don't care what input is, 
                        // as in: any char performs an action
    
    if(isDigit(ch) ) // is this an ascii digit between 0 and 9?
     {
       tempHolder = (10 * tempHolder) + (ch - '0'); // if yes, accumulate proper 
               digital value as int
     } //end if  
     else if (ch == 10)  // it's the newline character, same as '\n'
     {     
      blinkDelay = tempHolder;  //reconstructed integer
      Serial.print("# received, blink delay = "); Serial.println(blinkDelay);
      tempHolder = 0; // reset holder for next sequence to come in
     } //end else
   } //end while - repeats actions until Serial buffer empty,i.e. = 0

 convert serial input or char[] to String and back
 ------------------------------------------
 v. Read Serial Int C can use int i=atoi(char[]) or
 cast to String s = String(ch[]) then s.toInt()
 
 convert String object back to char[] from SFM Roo Comm 0.4
    sensorsAsString().toCharArray(data, 120);
    
 Simpler way to read int from serial input:
 ---------------------------------------
   if (Serial.available())
   { 
     delay(50);  // flaky reading of chars unless some delay, maybe <50 would work?
     int c = Serial.parseInt();  // will be a 2-3 digit int coming in giving temp in C.
     int nowInF = cenToFare(c);    // call convert function with temp C 
     Serial.print("F temp is "); Serial.println(nowInF);
    }  //end if serial.avail
  int cenToFare(int c)
   {
    return (c * 9/5 + 32)
   }
   
C ternary operator
------------------
IsLEDOn = !IsLEDOn; // toggle value of IsLEDOn
digitalWrite(LEDPin, IsLEDOn ? HIGH : LOW)

 ----------------------------------------------------
 //use of WHILE to hold action, and/or make looping function
  
  while(!Serial.available())  // if no serial input, holds the action here
  
  char ch=Serial.read();  // clears the buffer, assigns a value to ch
  
  while (!Serial.available()); {  // loops until typing any char(s), then should run body once
      repeat code here until something typed;
      Serial.read(); } // clears buffer so next loop stops at while above; may/not care about value

  while(ch != 'x')   // loop until x typed; 
    //  must be expression that evaluates to T/F, can it be a boolean variable by itself?
    {  
     delay(50);  // may help accuracy of read, could be much smaller ?
     ch = Serial.read();
     if(ch =='u')
       {
        mult=mult * 1.005;
        battVolt =analReed * mult;
       Serial.print("mult is now "); Serial.print(mult); 
       Serial.print("  volts read "); Serial.println(battVolt);
       }
     else if(ch =='d')
     {
       mult=mult * 0.995;
      battVolt =analReed * mult;
     Serial.print("mult is now "); Serial.print(mult); Serial.print("  volts read "); 
     Serial.println(battVolt);
     }  //end else
     delay(50);
    }  // end while, presumably only after an x comes
 
-------------------------------------------------------
  // works in basic C, also in Arduino (can, doesn't always, why?)
  
    // This executes the code in the while loop 5 times,
  int i = 5;
  while(i--)    // a 1 activates the loop, but then => 0, so that's the last execution
   {
      printf("java and c# can't do this");
   }

-------------------------------------------------------
// do - while loop, when you want it done at least once

do { some stuff
   }              // note final semicolon
while(condition);  // keep going if condx = true, else exit loop

------------------------------------------------------------------------ 
switch/case structure, from LCD1602
   lcd_key = read_LCD_buttons();   // read the buttons, put a number into variable for switch

   switch (lcd_key)
   {           // depending on which button was pushed, we perform an action;
               // indexes look like strings, but they are ints because of defines
       case btnRIGHT:
       {          //  if  "RIGHT"  pressed show this on the screen
            lcd.print("RIGHT ");
            break;
       }
       case btnLEFT:
       {
             lcd.print("LEFT "); //  if  "LEFT" show on the screen
             break;
       }    etc
       default:
       {break;}
   } end switch    
------------------------------------------------------------------------ 

 // char variables vs. their ascii number, from MorseTone player
 void loop()                    
{
  int potRead = analogRead(potPin); //map(input var, input min, in max, out min, out max)
  dotDelay = map(potRead,5,660,200,75);  // higher volts makes smaller delay = faster
  char ch;                // to hold indiv char to play from serial input
  static boolean var=0;  // just set it once when beginning
                         //otherwise if/while condx will delay each char
  delay(2000);
  if (!Serial.available())  // nothing in the buffer
       {    
       var=0;      // ser.avail will be > 0 while characters can be read from buffer;
                   // this resets to var to 0 when buffer empties again
       }
  if (Serial.available())   //same as (Ser.avail() > 0), value = # of bytes being held in buffer
  
 {                         // if/while condition pauses 4 sec
        if (var == 0)      // after new serial data comes
        {                  // before starting output
        delay(4000);
        var = 1;
        }
      ch = Serial.read();   //get char one by one from buffer, will be ascii value of char
  
      if (ch >= 'a' && ch <= 'z')           // 'z' is ascii value of z
      {    
        flashSequence(letters[ch - 'a']);  // so this gives index of ch in letter array
      }
      else if (ch >= 'A' && ch <= 'Z')
      {
        flashSequence(letters[ch - 'A']);
      }
      else if (ch >= '0' && ch <= '9')
      {
        flashSequence(numbers[ch - '0']);
      }
      else if (ch == ' ')
      {
        delay(dotDelay*4);  // gap between words  
      }
  }  //end if Ser.avail
  -----------------------------------------------------------------------
 // printing char vs its ascii value, int or byte, from MorseTone player, 
 v. Cookbook p.99-101
 // print random numbers with a fresh seed value for each sequence, but CAUTION
 // -- must put random() inside for (or some?) block to get sequence
     randomSeed(analogRead(0));  // read from an analog port with loose 
     wire connected
     for (byte i=0; i<7; i++)   // make 7 character words of random #s
    {                       
       ch = random(30);  // put random # 0-29 in ch
        if (ch <= 25)  // one of the 26 letters
        {    
          flashSequence(letters[ch]); //letters is array of char arrays holding morse code
          Serial.write(ch + 'A'), Serial.write(32);  // print the char + space
        }            // to print capital letter A-Z must add ascii value of A to ch value
                      // ser.print just prints the decimal # instead of its char value
                      // not sure why since ch is char type; maybe adding 'A' makes it an int?
        else if (ch >= 26  && ch <= 29)  //  4 slots for designating numbers
        {
          byte randY = random(0,256); //random(0,9) gave mostly 5
          randY = (randY % 10);   //want a random digit 0-9, hard getting it
          // Serial.print("randY is: "), Serial.println(randY); //for debugging
          flashSequence(numbers[randY]);    //numbers is array of char arrays for numbers       
        Serial.print(randY), Serial.write(32);  // print the # + space; Ser.print(32) prints 32
        }  // end else                        //randY is a byte so .print shows its numeric value
    }  // end 'for' word generation 

exponential moving average
-------------------------------
void loop() // expon MA automatically averages last N values, with older values decaying in importance
{
  static int prevAvg = analogRead(PinIn);  // 'static' sets it once, first loop only
  int voltOut;  // the averaged value you will output
          // read the input on analog pin 
  int sensorV = analogRead(PinIn);
  int newAvg = round((sensorV + 4*prevAvg)/5);  // updates expon moving average of sensor reading; may be faster to multiply than divide
  
  // use map fx to convert 0-1023 to voltage it corresponds to 0-5000 mV
  voltOut = map(newAvg,0,1023,0,5000);  // in mV, resolves ~5 mV

  // print out new values only if different
  if (newAvg != prevAvg) 
    { 
    Serial.print("pin out= "); 
    Serial.print(newAvg); 
    Serial.print(" calc mV= "); 
    Serial.println(voltOut);
    } // end if changed

  prevAvg = newAvg;   // reset value for next looping
  delay (20);   // if 50 loops / second, you are averaging 5 values over 100 mSec
}  // end loop

 // to print every 20 loops
----------------------------------------
  static byte counter = 20;   // static sets this once
  if(counter==20)   // print every 20 loops
    {
      Serial.println(String ("raw pin AR is " + myValue + " volts"));
      counter = 0;
    }
    counter++;

'concat' text and variable in one print statement
-----------------------------------------
// loop normally repeats, but not this one
void loop() 
{
  // read the input on some analog pin:
  int sensorValue = analogRead(A3); // done for clarity, could put aR(_) 
  into String(int)

  // convert the int to a String object var, not same as char string (a plain C array)
  String myValue = String(sensorValue,DEC);  // optional DEC formatter for int/flt
  // Serial.println("raw pin AR is " + myValue + " ticks"); // prints OK since 
myVal is String var
 can parse Str obj with charAt, setCharAt - v. String refs
//  this works too but is unnecessary
  Serial.println (String ("raw pin AR is " + myValue + " ticks"));  // concat of 
  Str's OK
  
// Serial.println("raw pin AR is " + sensorValue + " ticks"); // fails to compile w/ int var
// Serial.println("raw pin AR is "+ String(sensorValue) + " ticks"); //  but this works 
OK w/ conversion to String

// float not acceptable param for String constructor : v.i.

  delay(500);        // delay between prints
  exit(1); // exit(int) undocumented C fx, stops loop, sends int param to OS
} 

------------------------------------------
using string operators on float value, first convert it to string, then strcat, etc:
  -- from SerialReadWriteAM2,3
  float vFlt = 4.77 *((float)analogRead(0)/1023);  // need the cast from an int var
  char vStr[] = "0";    // make the buffer array, works; char vStr[6] ={0} maybe better init
        // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER);
  dtostrf (vFlt,4,3,vStr);      // makes float into a string
  sprintf(tempstr,"%s;",vStr);   // put string into tempstr
  strcat(out,tempstr);           // -- and appends it to out
  also works to convert int to char string
    sprintf(tempstr,"%d",someInt);   // put numbers into ch array buffer
    strcat(out,tempstr)          // add buffer string to array
  
 or print(flote,3) -- to print 3 digit precis
 
or #INCLUDE "Streaming.h" (download, add to user 'libraries') to use C++ insertions 
  in any Stream derived class:
  -- from SeriReadWritAM2; this prints to your Ser. Mon. or lcd
lcd << "some var =  " << var << "units"<< endl or '\n'; // effect of \n in lcd print?
Serial << "outstr= " << out << '\n';  // OK to insert single char too
// with <Streaming.h> included, can print the float var by insertion directly
// tho this adds ~1.5Kb to your sketch size;
Serial << "and flot2 = " << vFlt << " volts" << endl; 
// float print defaults to 2 digits; for 3 digit precision use _FLOAT(vFlt,3)  
// other formatting prefixes for numbers: _BIN, _HEX, _DEC defined in the streaming.h
float vFlt = analogRead(A0)/5.0;  this prints to lcd with 1 # after decimal
lcd << "raw AR " << _FLOAT(vFlt,1);
------------------------------------------------------------------------ 
reading a char string one by one using pointer to it
 for(const char *p = "some string\n"; c = *p; p++)  // not sure if const type needed or why
 doSomething(c);  // doesSometg to each c in string until c=*p goes false when out of chars
 
 // does c need a type? no
------------------------------------------------------------------------ 
 enhanced if, ternary, conditional operator: from SFM RooCoom v0.3
     // gentler turns if going fast c.f. C prog p 116
    int spd = (speed > 160) ? round(speed * 0.8) : speed;
------------------------------------------------------------------------ 

using flash memory to store some program data like strings that doesn't change much

from my makingTTerrata.txt
p. 346, "Saving Program Memory": Using the F() syntax to store string
constants in this sketch is a very good idea, but the text misstates the
reason and gets some fundamentals wrong. It's not Program Memory (flash)
you are trying to save -- it's the RAM used by the program! The Uno's
program memory (aka flash) is 32K, whereas its RAM (aka SRAM) is 2K, and
strings take up a lot of room. The F("string") operator stores them in
program memory, leaving more room in RAM for program operations. So the
paragraph should read:

"Saving RAM for your program
The sketch for this project is complex, and it takes a large chunk of
the Arduino's (32K) program memory, but it puts even more strain on the
limited RAM (2K) available to run it. You'll notice that the Serial
print statements use a new syntax: Serial.println(F("Hello")).
The F() notation tells the program to store the text that follows in
flash (i.e. program memory) rather than RAM. Because the print
statements are for debugging only, and are not going to change, this
saves precious RAM memory for your program to use."

see: http://playground.arduino.cc/Learning/Memory

----------------------------------------
put function in its own tab to improve code readability; name tab something
nice and save; tab will be made into .ino file in same folder as sketch (may
not need to be #included ?)

eg my MorseToneRandTabbed; eg. below SFM Roo Comm uses many .h files
for images and constants; images converted using xxd -i old.png > new.h

or if you have a bunch of constants or functions they can go into their own
header.h (text) file in sketchFldr/Libraries/ which you then #include 
@ top of sketch
----------------------------------------
debounce an Interrupt signal inside the ISR
   << from turrentInterruptC
void my_interrupt_handler()
{
 static unsigned long last_interrupt_time = 0;  // static is set once, not changed by 
          repeated function calls
// or make it a global to respect another interrupt
 unsigned long interrupt_time = millis();       //  millis() is not updated inside ISR but OK 
             to use its value at instant of interrupt
 
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 200)
    {
      ... do your thing
       last_interrupt_time = interrupt_time;  // should be reset only on good signal, 
       > 200 ms from a valid one
    }
}

 another simple interrupt debounce from Instructables:
---------------------------------------------------
 
 //Software debouncing in Interrupt, by Delphiño K.M.

 long debouncing_time = 15; //Debouncing Time in Ms  // no good reason to make long
 volatile unsigned long last_micros;    // global, used in 2 functions; I'd use millis()

 void setup() {
   attachInterrupt(0, debounceInterrupt, RISING);   // prefer FALLING if possible
 }

 void loop() {
 }

 void debounceInterrupt() {
   if((long)(micros() - last_micros) >= debouncing_time * 1000) {
     Interrupt();
     last_micros = micros();
   }
 }

 void Interrupt() {
   //Do Something
 }
 
hardware based timer, may work better, or not; 
? if timer0 works inside Interrupt function
------------------------------------------------------
// ISR Debounce

// use 2ms debounce time
#define DEBOUNCE_TICKS (word)microsecondsToClockCycles(2000)

extern volatile unsigned long timer0_overflow_count;  // set in hardware
word keytick;  // record time of keypress      // is this init. to 0 by default?

void setup() 
  {                
     attachInterrupt(0,KeyPress,FALLING);
  }
       //interrupt calls KeyPress, which puts overflow count at that moment into keytick
void KeyPress() 
   {
    keytick=(word)timer0_overflow_count;  // each interrupt gives it a new value
   }

// returns true if key pressed
boolean KeyCheck()   // this fx runs in main loop, polling overflow - keytick
 {
    if (keytick!=0)    // would be 0 on 1st run or after below runs, i.e. ready to reevaluate
     {
       if (((word)timer0_overflow_count - keytick)>DEBOUNCE_TICKS) 
          {
            keytick=0;       // restart the count
            return true;
           }  //end word if
      }  // end keytick if
    return false;     // false unless 1st run or enough time has passed
 }  // end function

void loop()                     
{
   if (KeyCheck()) 
   {
     // do something useful
   }
}

FastLED color assignments
-------------------------
usually just: 
 leds[1]= CRGB::Red, or = CRGB(0x99ffff)
but if you have to assign using a var (e.g. w/ options in setup)
1st init CRGB C; // color to send lites, which seem to be byte triplets
in setup: C = CRGB::Red, C = CRGB(255, 0, 0), C = CRGB(0x99ffff);
then in loop: leds[0], leds[1] = C;

FastLED v>3.1 special timer fx
-----------------------
   EVERY_N_SECONDS(5) || EVERY_N_MILLISECONDS(5) 
   { // doo stuff
    T = millis();
    Serial.println ("pulse " + String (pulse) + " millis " + String(T)); 
   }
more:https://plus.google.com/112916219338292742137/posts/ZCucuzvCJFd

#include <TimedAction.h>  -- threadlike non-delaying events
-----------------------
how to use in lib and my sketch AnalInputMap2ToneC
TimedAction beepONoff = TimedAction(1000,beep); interval resetable


#include <elapsedMillis.h>  // to do things like .print at interval
-----------------------  used in vl6180 et al
elapsedMillis timeElapsed; // declare global so not reset every loop
could you declare static in loop?
unsigned int interval = 1000;  // ms
in loop()
if (timeElapsed > interval) 
  do stuff @ interval ... reset var
  timeElapsed = 0;
