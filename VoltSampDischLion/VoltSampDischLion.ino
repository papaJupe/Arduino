/*
  VoltSampDischLion, modif of VoltageSamplerDisch, to test graphic display of
  simulated Lion DC using AR of pot; faux curr monitor

  set up Ardu with pot on Anal pins, test w/ Ser Mon, if OK, test reading V with Proc sketch
*/

unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals fx
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global, same reason)

// int analReed;   to simulate voltage value read from an analog pin
const int targVolt = 11400; // discharge target in mV
// for 8x2 LCD, include the library
#include <LiquidCrystal.h>
// initialize new lcd object with the Arduino pins
LiquidCrystal lcd(12, 11, 4, 5, 6, 7); // lib is smart enough to know if you're using 4 pins or 8

#define potPin A3          // pin to read test batt voltage

int mapA = 100;           // default V map params, A for 11.5, B for 13.0
int mapB = 1000;           // globals, used in calib and dc readout

int newVavg;
int prevVavg;              // globals, used in MA calc in setup calib and dc readout
int battVolt;

// ************************************

void setup()   // check & calibrate voltage readout, if OK sets starTime for dc readout
{
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(8, 2);

  pinMode(potPin, INPUT);   // put pot ends/device to 5v and grnd, signal voltage to A3
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);

  prevVavg = analogRead(potPin);  // used for MA of voltage in loop

  battVolt = map(prevVavg, mapA, mapB, 11500, 13000); // in mV, resolves ~2 mV


  // print to 2 lines of LCD, if it's present, once only
  lcd.clear();  // first clear screen and set the cursor to (0,0)
  lcd.print(battVolt);
  lcd.print(" mV");  // the averaged voltage

  starTime = millis();     // start of data collection, set once
  prevTime = starTime;    // used in interval printing and elapsed time
  printVals(battVolt);    // print first time/value set

  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);

}    //end setup

void loop()   // read the batt volt 5/sec, print out time, values, every 3 sec.
{
  // read the analog input pin, average it over 5 loops

  int pinTemp = analogRead(potPin);
  newVavg = round(pinTemp * 0.2 + prevVavg * 0.8);  //update expon moving average of AR
  prevVavg = newVavg;

  uint16_t interval = (millis() - prevTime);
  // ms. elapsed since loop start or last printVal job

  // print vals every few sec
  if (interval >= 3000)
  {
    // convert avg analog reading (0 - 1023) to voltage (11.5-13) using map params
    battVolt = map(newVavg, mapA, mapB, 11500, 13000); // in mV, resolves ~2 mV
    printVals(battVolt);

    // print to 2 lines of LCD, if it's present every 3 sec
    lcd.clear();  // first clear screen and set the cursor to (0,0)
    lcd.print(battVolt), lcd.print(" mV");  // the averaged voltage

    prevTime = millis();
  }

  delay(200);

}  //end main loop


// *******************************
void printVals(int battV)    // Serial prints time, batt in mV, faux mA
{
  // Serial.print("elapsed sec.= ");  // comment out for graphing
  Serial.print((millis() - starTime) / 1000);
  Serial.print(",");
  //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees);
  //Serial.print("  batt= "); // comment out for graphing
  Serial.print(battV);
  Serial.print(",");
  //Serial.print(" mV"); // comment out for graphing
  Serial.println("faux42");
  //Serial.println(" mA");  // comment out for graphing

}  //end printVals






