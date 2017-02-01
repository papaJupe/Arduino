/*
  SerialInput2lcd -- recv data over serial port (from Proc., time + Air Quality), 
  display it with lcd

 */

//  include the library and initialize 8x2 LCD
#include <LiquidCrystal.h>
// initialize new object with the Arduino pins it uses
LiquidCrystal lcd(12,11,4,5,6,7);  // lib configures for 4 pins or 8 via overloads

// init the char arrays for incoming
char arrayT[9] = {'?'}; // one value at [0], others should be null
char arrayA[9] = {'#'};  // same

void setup() 
  {
    // initialize serial communication at 9600 bps:
    Serial.begin(9600);
    // set up the LCD's number of columns and rows: 
    lcd.begin(8,2);
     //printLiq(); // works here 1st time, not if used later, again, or 1st below
    lcd.clear();  // clear screen and set cursor to (0,0)       
    lcd.print("data:");
    lcd.print(arrayT);  // the time string, now just a ?
    lcd.setCursor(0,1);   // second line
    lcd.print("AQI:"); 
    lcd.print(arrayA);  // the AQI string, just a # now
  }   // end setup

/* or could capture entire stream then parse
                     // .readBytes(buffer,length) puts (length) bytes into (buffer) ch array
  Serial.readBytes(in,Serial.available());  // reads all bytes in Serial buffer 
*/
void loop() 
{    
  while(!Serial.available());  // while no serial input, hold the action, then
  Serial.read();  // reads, clears wake-up byte
  // clear the arrays by reinit after new data (wake up) arrives
  char arrayT[9] = {"incompl"}; // loads value at [0], others should be null
  char arrayA[9] = {"data"};  // same; may show in lcd if incompl data
     // printLiq(); // wont work here, 1st use or not
   Serial.println(" -- wake up recd");  // each new data shows this 3 sec
   delay(3000); // may stop here if only wake up byte sent, lcd print inc. data
  if (Serial.available())  // 
   {    // incoming strings, first (time \n) then AQI, each <= 8 data char to print
     delay(10);
     char ch = Serial.read(); 
     while ((ch != 10) && (ch !=122))  // newline (or z for debug) marks end of time data
       {   //read and put chars into arrayT
       for (byte i=0; i < 9; i++)  // does plain ch[] have string term. \0, null by default
         {
         arrayT[i] = ch; // 1st char received now -> elem[0]
     
         // Serial.print(" putin arrayT "); Serial.println(ch);
         ch = Serial.read();  // read next one
         delay(10);
         if ((ch == 10) || (ch == 122)) break;  // \n or z is end of time data, so we break
         } // end for                        // to while loop and exit it too 
       }  // exit while loop, so ch = \n now ?, so read next char from stream
       Serial.println(" //end while");
        
      ch = Serial.read();  // next char should be A, start of AQI data
      // if it's not, try next ch
      if (ch != 'A') ch = Serial.read(); // or could use do/while to keep going
      if (ch == 'A')   // starts AQI data, so put it in array
       {  // put A plus next <=7 char into arrayA
        for (byte i = 0; i < 9; i++)  //  fill up 0-8 with something
         {
          if((isAlphaNumeric(ch)) || (isPunct(ch))) arrayA[i] = ch; // no controls etc
            else  // end of data, put spaces in remaining slots, could be any nonprinting
                arrayA[i] = 32;  // put space if not a good char
          // Serial.print(" putin arrayA "); Serial.println(ch,DEC); // as DEC shows ASCII number
          if (!Serial.available()) break; // nuffin left, exit for loop. otherwise read next ch
          ch = Serial.read();  // get next char in stream
        
          } // end for
       }   // end if A
    }  //end if serial.avail
    
     // printLiq();  // don't know why it fails here and not in setup
     // print the arrays anyway, just "incompl data" if data writes failed
    lcd.clear();  // clear screen and set cursor to (0,0)       
     //lcd.print("pin=");
    lcd.print(arrayT);  // the time string
    lcd.setCursor(0,1);   // second line
     //lcd.print("mV="); 
    lcd.print(arrayA);  // the AQI string
    Serial.println("lcd done");
    delay (200);   // five loops / second
}  // end loop

   
    // print to 2 lines of LCD, if it's present -- won't work except in setup
 void printLiq()      // ? because it's using an external class
    {     // only should get here if new serial data or end of new array fill
    lcd.clear();  // clear screen and set cursor to (0,0)       
    //lcd.print("pin=");
    lcd.print(arrayT);  // the time string 
    lcd.setCursor(0,1);   // second line
     //lcd.print("mV="); 
    lcd.print(arrayA);  // the AQI string
    } // end printL





