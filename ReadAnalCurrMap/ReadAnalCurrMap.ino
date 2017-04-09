
/*ReadAnalCurrMap also copy in voltSampB2 folder, will it open with it?
  //   reads analog input on pin X, and prints the result
  //   to the serial monitor if it has changed; uses mapping values to map
  //   pin AR to actual current (voltage across 2 ohm) going to op-amp A.
  //   Uses: analRead, expon moving averag, map, round
*/

// for 8x2 LCD, include the library
#include <LiquidCrystal.h>
// initialize new lcd object with the Arduino pins
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

float newAvg;
float prevAvg;          // globals, appear in setup and loop
float indxAvg;

// float pinSees;
// int current;  local vars in loop only

#define PinIn A2

// setup runs once
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(8, 2);
  pinMode(16, INPUT);   // same as A2
  prevAvg = analogRead(PinIn);  // just some starting place for the MA
  //  indxAvg = prevAvg;          // init this value for MA calc in loop
}

void loop()
{
  // read the input on current analog pin, use moving average to stabilize
  float pinSees = analogRead(PinIn);
  newAvg = pinSees * 0.2 + prevAvg * 0.8; // update expon moving average of AR
  // map AR ticks to current (mA)
  // convert analog reading (0 - 1023) to current (via amplified voltage (0-1.9 across Rc)

  //int current = map(newAvg,575,948,600,900);  //  Rl 9.2
  // int current = map(newAvg,586,849,600,800);  //  Rl 14
  // int current = map(newAvg,210,656,300,650);  // with Rc 2, Rl 16
  int current = map(newAvg, 109, 320, 200, 400); // with Rc 2, Rl 27
  // int current = map(newAvg,168,504,200,400); //for 2.7 ohm Rc 27 ohm Rl
  // int current = map(newAvg,121,625,80,250);  //with 5 Rc and 36 Rl(adj+2+5 in series)

  // print the new MA if changed
  if (round(newAvg) != round(prevAvg))
  {
    Serial.print("pin out= ");
    Serial.print(newAvg, 0); // int part of float only
    Serial.print(" calc curr = ");
    Serial.println(current);
    // indxAvg = newAvg;
    // print raw pin reads to LCD
    lcd.clear();  // first clear screen
    lcd.print("cPin " + String(round(newAvg)));
    lcd.setCursor(0, 1);  // second line
    lcd.print("curr " + String(current));
  }
  prevAvg = newAvg;
  delay (200);   // five loops / second
}
