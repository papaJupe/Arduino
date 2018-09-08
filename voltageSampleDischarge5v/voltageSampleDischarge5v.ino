/*
  VoltageSamplerDisch5v, modif of VoltageSamplerDc, rough monitor discharge,
  using external R, of Lion pk segment to 3.4 v. using 16x2 LCD
  -- first get map values from sketch like ReadAnalogCalMap w/ pot input
*/

unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global)

const int targVolt = 3300; // discharge target in mV for single lion segment

#include <LiquidCrystal.h>
// initialize new lcd object with the Arduino pins for 16x2 LCD shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // lib  knows if you're using 4 pins or 8

#define pinIn A4          // pin to read batt voltage

int mapA = 759;        // default V map params, A for 3.2 B for 4.4
int mapB = 937;        // globals, used in calib and dc readout
int loV = 3400;        // range of V to map
int hiV = 4200;

float battVolt;
float newVavg;
float prevVavg;      // voltage globals, used in MA calc in setup calib and dc readout
float indxVavg;

// ************************************

void setup()   // if accurate output, exit to readout phase by typing x
// in SerMon; sets starTime for dc readout et al
{
  Serial.begin(9600);
  // set LCD number of columns and rows
  lcd.begin(16, 2);

  int voltage;         // local var, just used in setup
  pinMode(pinIn, INPUT);  // read batt voltage up to 5 v.
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);  // v source for calib pot
  delay(2000);         //  wait to get first samples

  prevVavg = analogRead(pinIn);  //just a starting place for the MA of voltage
  indxVavg = 1026;      // higher than possible, so 'do' prints immediately

  readPinAvg();  // puts averaged pinIn read in newVavg global var used in map()

  battVolt = map(newVavg, mapA, mapB, loV, hiV); // in mV, resolves 5 mV

  // print to LCD, if it's present
  lcd.clear();  // clears screen and sets cursor to (0,0)
  lcd.print((int)battVolt);   // the averaged voltage
  lcd.print(" mV (setup)");

  Serial.println("printing pin reading & calcul batt V");
  Serial.println("type x to start recording");
  // x exits setup, begins recording & printout

  /************************************/

  do   // it at least once
  {
    readPinAvg();    // gets avg of V pins over 1 sec., updates globals

    // convert analog reading (0 - 1023) to batt voltage using map params
    voltage = map(newVavg, mapA, mapB, loV, hiV); // in mV, resolves 5 mV
    // print a new MA if changed, reset indexes
    if (newVavg != indxVavg)
    {
      Serial.print("pin sees= ");
      Serial.print(newVavg);
      Serial.print(" calc V= ");
      Serial.println(voltage);

      lcd.clear();  // first clear screen and set the cursor to (0,0)
      lcd.print(voltage);
      lcd.print(" mV");  // the averaged voltage

      indxVavg = newVavg;
    }  // end if different
  }  // end do/while on 'x' -- you either like the output or want to enter new map vals
  while (Serial.read() != 'x'); // while no serial x input, keep doing the do

  delay(100);

  starTime = millis();     // start of data collection, set once
  prevTime = starTime;    // set once here, then reset by each print job
  printVals(voltage);    // w/ starTime defined, now print first time/V. set

}    //end setup

void loop()   // read the batt volt, print out time, values, more often as V drops
{
  static boolean done = 0;       // i.e. 0 = not done yet, init to 0 first loop only
  // static boolean watch = 0;        // whether to continue plot after target reached
  static unsigned long interval = 0;

  // read the analog input pin
  readPinAvg();  // gets average V over 2 sec, puts it in newXavg global variable

  // convert analog reading (0 - 1023) to a voltage using map params
  battVolt = map(newVavg, mapA, mapB, loV, hiV); // in mV, resolves 5 mV

  static int baseVolt = (int)battVolt;      // batt V in mV when we started discharging, set once
  static int battRange = (baseVolt - targVolt);  // range from starting high to target V, set once

  while (done != 1) // do this until done is true (target V reached)
  {
    interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
    int deltaV;           // how high present V is from target in mV
    // 2 local vars, only used here, will determine whether to print or wait
    // Serial.print("interval now  "); Serial.println (interval);

    // refresh battVolt 'while' looping
    readPinAvg();  // gets average V, puts in newXavg global variable

    battVolt = map(newVavg, mapA, mapB, loV, hiV); // in mV, resolves ~2 mV


    // print to LCD, if it's present, every 2.5 sec, while loop + reading delay
    lcd.clear();  // first clear screen and set the cursor to (0,0)
    lcd.print(battVolt);
    lcd.print(" mV");  // the averaged voltage

    deltaV  = (battVolt - targVolt);  //  how far batt is from target now
    //   Serial.print("delta  "); Serial.println(deltaV);   // should drop as batt dc's

    if ( (deltaV > round(battRange * 0.5)) && (interval >= 15 * 60000) )
      printVals(battVolt);  //print every 15 min while far above target V
    // rounding should give nearest integer to product
    if ( (deltaV <= round(battRange * 0.5)) && (deltaV > round(battRange * 0.25)) && (interval >= 10 * 60000) )
      printVals(battVolt);    //print every 10 min in this range

    if ( (deltaV <= round(battRange * 0.25)) && (deltaV > round(battRange * 0.13)) && (interval >= 5 * 60000) )
      printVals(battVolt);    //print every 5 min

    if ( (deltaV <= round(battRange * 0.13)) && (deltaV > round(battRange * 0.06)) && (interval >= 3 * 60000) )
      printVals(battVolt);    //print every 3 min

    if ( (deltaV <= round(battRange * 0.06)) && (deltaV >= 1) && (interval >= 60000))
      printVals(battVolt);    //print every 1 min

    if (deltaV < 1)     // target V reached
    {
      done = 1;        // stops 'while' loop cycling, exits to main loop
      break;
    }
    delay(500);      // while, you wait
  }  // end while loop, must be done   // Serial.println("exit while loop")

  // you're done, keep printing to Ser Mon & LCD w/ main loop every 2.5 sec
  interval = (millis() - prevTime);   // ms. elapsed since last printVal job

  readPinAvg();  // gets average of V pin, puts in newXavg global variable

  battVolt = map(newVavg, mapA, mapB, loV, hiV); // in mV, resolves 5 mV
  if (interval >=  60000) printVals(battVolt);  // print to ser mon every min

  lcd.clear();  // clear screen and set cursor to (0,0)
  lcd.print("V < target now");
  lcd.setCursor(0, 1);            // move to the begining of the second line
  lcd.print(battVolt), lcd.print(" mV");  // the averaged voltage

  /*
    if (done == 1 && watch == 0)  // main loop does this block when done is true
    {       // print a fresh value
      Serial.print("elapsed min.= "), Serial.println((millis() - starTime) / 60000);
      Serial.print(battVolt);
      Serial.println("  -- discharge target reached");
      // Serial.println ("NOW - STOP ME WHILE YOU CAN!");
      Serial.println ("for extended dc type e immediately");
      delay(5000);
      if (Serial.read() == 'e')
      {
        Serial.println("values print to ser. mon. every 3 min.");
        watch = 1;
      }

      if (watch == 0)  // don't beep if watching
      {
        for (byte i=0; i<=6; i++)  // beep tone every 20 sec
        {
          tone(8, 2700);   // don't need pin config for toner
          delay(500);
          noTone(8);
          delay(200);
        } // end tone loop
        delay (18000);   // 2 + 18 = 20 sec between plays
      }
    }  //end if done

  */

  delay(500);

}  //end main loop

void readPinAvg()  // get average of 5 readings over 2 sec., update globals
{
  prevVavg = analogRead(pinIn);      // just a starting place for the MA of V
  for (int i = 0; i < 5; i++)
  {
    float pin1Sees = analogRead(pinIn);
    newVavg = pin1Sees*0.2 + prevVavg*0.8; //update expon moving average of aR
    prevVavg = newVavg;

    delay(400);
  }  // end for
}  // end readPinAvg

// *******************************
void printVals(int battV)    // Serial prints batt in mV, resets prevTime
{
  Serial.print("elapsed min.= ");
  Serial.print((millis() - starTime) / 60000);
  //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees);
  Serial.print("  batt= ");
  Serial.print(battV);
  Serial.println(" mV");
  prevTime = millis();
}  //end printVals






