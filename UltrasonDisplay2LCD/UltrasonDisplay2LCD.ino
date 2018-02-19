/*
  US (analog) display distance to LCD
   -- averages analog input on pin, voltage or some mapped value, display to LCD
  Attach the center pin of pot to PinIn, and the outside pins to +5V and ground,
  -- can use adjacent anal pins to supply gnd and 5v. prn
  Uses: Liquid Crystal Nx2 display, map, analRead, expon moving average, round
*/

#define PinIn A5    // analog pin In

//  include the library and init for 8x2 or 16x2 LCD
#include <LiquidCrystal.h>
// initialize new object with the Arduino pins it uses
// lib knows if you're using 4 pins or 8
// LiquidCrystal lcd(12, 11, 4, 5, 6, 7);   // 8x2
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  //  16x2

void setup()
{
  // initialize serial communication at 9600 bps:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(PinIn, INPUT);
  //  pinMode(A1, OUTPUT); // can use 2 anal pins as voltage source for device
  //  pinMode(A2, OUTPUT);
  //  digitalWrite(A2, LOW); // grnd
  //  digitalWrite(A1, HIGH); // '5' v for device
}   // end setup

void loop()
{
  static int prevAvg = analogRead(PinIn) + 5; // just set once, first loop
  int voltage;
  // read the input on analog pin
  int sensorV = analogRead(PinIn);
  int newAvg = round((sensorV * 0.2) + (prevAvg * 0.8));  // update expon moving average of AR

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5000mV):
  // voltage = (int)(newAvg * 4.875);  // in mV, resolves about 5 mV; cast to int (mV)
  // 4.xx=empiric multiplier at midrange; varies some from pin to pin, board to board
  // or use map from ~10mV 2 ticks / inch, range ~ 6" to 360"
  voltage = map(newAvg, 21, 204, 7, 79); // mapping ticks to inches for MaxBot MB1010

  // print out new values if different
  if (newAvg != prevAvg)  // only print if it's changed
  { //print to Ser Mon if it's on
    Serial.print("pin out= ");
    Serial.print(newAvg);
    Serial.print(" calc mV= ");
    Serial.println(voltage);

    // print to 2 lines of LCD, if it's present
    lcd.clear();  // first clear screen and set the cursor to (0,0)
    lcd.print("pin = ");
    lcd.print(newAvg);  // the averaged raw AR
    lcd.setCursor(0, 1);  // second line
    lcd.print("MB : ");
    lcd.print(voltage);
    lcd.print(" in.");
    prevAvg = newAvg;
  } // end if changed


  delay (200);   // five loops / second
}  // end loop
