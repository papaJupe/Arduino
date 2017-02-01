/*
 ReadAnalogVaverage - averages analog input on pin X, calcul actual 0-5000mV, and 
 prints the result to (ser. mon. +/- lcd if present) if it has changed; plot this against 
 measured input voltage by voltmeter. 
 Use to calculate mult or map for pin reading of voltage, checking various A pins, monitor
 batt discharge; added tone when dc target V reached for small < 5 v batts.
 Attach the center pin of a potentiometer to PinIn, and the outside pins to +5V and ground.
 Uses: Liquid Crystal 8x2 display, map, analRead, expon moving average, round, toner
 */

#define PinIn A0    // analog pin In

//  include the library and init for 8x2 LCD
#include <LiquidCrystal.h>
// initialize new object with the Arduino pins it uses
LiquidCrystal lcd(12,11,4,5,6,7);  //lib is smart enough to know if you're using 4 pins or 8

void setup() 
{
  // initialize serial communication at 9600 bps:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(8,2);

  pinMode(14, INPUT);   // same as A0
  pinMode(15, INPUT);   // may be redundant -- (A pin defaults)
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);   // same as A5
}   // end setup

void loop() 
{
  static int prevAvg = analogRead(PinIn)+5;  // just set once, first loop
  int voltage;
  // read the input on analog pin 
  int sensorV = analogRead(PinIn);
  int newAvg = round((sensorV + 4*prevAvg)/5);  //updates expon moving average of AR
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5000mV):
  // voltage = (int)(newAvg * 4.875);  // in mV, resolves about 5 mV; cast to int (mV)
  // 4.xx=empiric multiplier at midrange; varies some from pin to pin, board to board
  // or use map  -- seems more accurate over range
  voltage = map(newAvg,46,1019,250,4950);  // in mV, resolves ~5 mV

  // print out new values if different
  if (newAvg != prevAvg)  // only print if it's changed
    {     //print to Ser Mon if it's on
    Serial.print("pin out= "); 
    Serial.print(newAvg); 
    Serial.print(" calc mV= "); 
    Serial.println(voltage);
  
    // print to 2 lines of LCD, if it's present
    lcd.clear();  // first clear screen and set the cursor to (0,0)       
    lcd.print("pin=");
    lcd.print(newAvg);  // the averaged raw AR 
    lcd.setCursor(0,1);   // second line
    lcd.print("mV="); 
    lcd.print(voltage);
  
    prevAvg = newAvg;
    } // end if changed

  //  if (voltage <= 900)  // target for 1.2 v NiCd cells
  //  {       // print a fresh value
  //    Serial.print(voltage); 
  //    Serial.println("  -- discharge target reached");  
  //    Serial.println ("NOW - STOP ME WHILE YOU CAN!");
  //    for (byte i=0; i<5; i++)  // play finish tone every 20 sec
  //    {
  //      tone(8, 2700);   // don't need pin config for toner
  //      delay(500);
  //      noTone(8);
  //      delay(200);
  //    } // end tone loop
  //    delay (5000);   // 5 sec between plays
  //  }  //end terminal if

  delay (200);   // five loops / second
}  // end loop
