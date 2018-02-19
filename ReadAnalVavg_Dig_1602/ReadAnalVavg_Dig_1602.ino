/*
ReadAnalogVavg_Dig1602 -- reads an averaged analog input on pin X, and prints the result 
  0-5000mV (+/- ser. mon. +/- lcd if present) if it has changed. Attach the center pin of  
  potentiometer to PinIn, and the outside pins to +5V and ground, or any other analog input.

  Also takes one hi/lo input to D3 and LED displays on/off from D11. Like ReadAnalVavgDig, 
  modif for 16x2 display
  Uses: Liquid Crystal libr, 16x2 display, DFR keypad libr, reading key value
*/

#define PinIn A2        // analog 0 is wired to switch array on 1602 board
#define DigIn 3       // digital pin 3
#define LiteOut 11  // LED output

    // include the library code for LCD
#include <LiquidCrystal.h>

  // switches feed a range of voltages to A0, so you could use them somehow,
  // or use the library to send you back an int to use in a switch/case function

// initialize new 16x2 lcd with the Arduino pins
LiquidCrystal lcd(8,9,4,5,6,7);  // lib adapts to using 4 data pins or 8
                 
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16,2);
  pinMode(DigIn, INPUT);
  digitalWrite(DigIn,HIGH);   // internal pull-up, or use INPUT_PULLUP
  pinMode(LiteOut, OUTPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
   // lcd.print("Key Grab v0.2");
  delay(2000);
}  // end setup

void loop() 
{
// localKey = keypad.getKey();  // returns an int 0-5, depending
  
//  if (localKey != SAMPLE_WAIT)  // a -1 value if no new data
//  {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Current Key:");
//    lcd.setCursor(0, 1);
//    lcd.print(localKey)  // the number of key pressed
//  } // end if
//  
  // if switch pulls down the input, it turns ON the LED
  digitalWrite(LiteOut, !digitalRead(DigIn)); 

  static int prevAvg = analogRead(PinIn);  // just set once
  int voltage;
  // read the input on analog pin 
  int sensorV = analogRead(PinIn);
  int newAvg = round((sensorV + 4*prevAvg)/5);  // updates expon moving average of AR
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5000 mV):
  voltage = (int)(newAvg * 4.77);  // in mV, resolves about 5 mV; cast to int (mV)
  // empiric multiplier at midrange; varies some from pin to pin, board to board
  // print out the new MA if different
  if (newAvg != prevAvg)
  {    //print to Ser Mon if it's on
    Serial.print("anal pin = "); 
    Serial.print(newAvg); 
    Serial.print(" calc mV= "); 
    Serial.println(voltage);

    // print to 2 lines of LCD, if it's present
    lcd.clear();  // first clear screen and set the cursor to (0,0)       
    lcd.print("pin=");
    lcd.print(newAvg);  // the averaged AR 
    lcd.setCursor(0,1);   // second line
    lcd.print("mV="); 
    lcd.print(voltage);

    prevAvg = newAvg;

  } // end if changed value
  delay (100);   // ten loops / second 
}  // end loop
