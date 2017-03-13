/*
  ReadAnalogVaverage_Dig -  reads an averaged analog input on pin X, and prints the result 0-5000mV
    (+/- ser. mon. +/- lcd if present) if it has changed.  Attach the center pin of a potentiometer
    to PinIn, and the outside pins to +5V and ground; or any analog source.
    Also takes one hi/lo input to D3 and LED displays on/off from D8 -- used to test anal and dig
    IR sensors et. al.
    Uses: Liquid Crystal lcd, round, expon moving avg, flt to int cast, map
*/

#define PinIn 2      // analog 2
#define DigIn 15     // digital pin IN
#define LiteOut 13  // LED output

// include the library code for the LCD
//#include <LiquidCrystal.h>
//// initialize new 8x2 display with the Arduino pins
//LiquidCrystal lcd(12,11,4,5,6,7);  //lib is smart enough to know if you're using 4 pins or 8

void setup()
{
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);
  // set up the LCD's number of columns and rows
  //lcd.begin(8,2);
  pinMode(DigIn, INPUT_PULLUP);
  pinMode(LiteOut, OUTPUT);
}  // end setup

void loop()
{
  // digitalWrite(LiteOut, !digitalRead(DigIn));  // if switch pulls down the input, it turns ON the LED
  digitalWrite(LiteOut, digitalRead(DigIn));  // if dig input goes high, so does LED output
  static int prevAvg = analogRead(PinIn);  // just set once
  int voltage;
  // read the input on analog pin
  int sensorV = analogRead(PinIn);
  int newAvg = round((sensorV + 4 * prevAvg) / 5); //updates expon moving average of AR
  // to integer nearest the float
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5000 mV):
  // voltage = (int)(newAvg * 4.77);  // in mV, resolves about 5 mV; cast to int (mV)
  // empiric multiplier at midrange; varies some from pin to pin, board to board
  // or use map  -- seems more accurate over range
  voltage = map(newAvg, 46, 1019, 250, 4950); // in mV, resolves ~5 mV
  // print out the new average if different
  if (newAvg != prevAvg)  // print only if changed
  { //print to Ser Mon if it's on
//    Serial.print("pin out= ");
//    Serial.print(newAvg);
//    Serial.print(" calc mV= ");
//    Serial.println(voltage);

    // print to 2 lines of LCD, if it's present
//    lcd.clear();             // first clear screen and set the cursor to (0,0)
//    lcd.print("pin=");
//    lcd.print(newAvg);  // the averaged AR
//    lcd.setCursor(0, 1);  // second line
//    lcd.print("mV=");
//    lcd.print(voltage);
//
//    prevAvg = newAvg;

  } // end if changed value
  delay (100);   // ten loops / second
}  // end loop
