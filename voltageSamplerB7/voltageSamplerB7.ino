
/*
  VoltageSamplerB7, modif of VoltageSamplerB6 for serial output to Proc sketch
   of Lion batt dc
   
  -- just the numbers for min,mV,mA
 
  -- converts 0-5 v from diff. op-amps to read ~11.5-13 v from batt source
  and Rcurr drop to current draw; [calib phase for V in setup]; Curr map params
  come from previous calib sketch -- do that first with current Rc and load Rl
  being used
*/


unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals fx
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global, same reason)

const int targVolt = 11500; // discharge target in mV, varies with batt type, depth you want;
// this is 20% dc at C/20 for SLA; variable used only in loop, init here for clarity

// for 8x2 LCD, include the library
#include <LiquidCrystal.h>
// initialize new lcd object with the Arduino pins
LiquidCrystal lcd(12, 11, 4, 5, 6, 7); // lib is smart enough to know if you're using 4 or 8 pins

#define pinIn1 A5          // pin to read test batt voltage via op-amplification
#define pinIn2 A2          // pin to read curr sense voltage across Rc via op-amplification

int mapA = 123;      // default V map params, A for 11.5, B for 13.0
int mapB = 975;

//int mapC = 640;      // empiric C map params, pinIn2 to dc current mA
//int mapD = 928;      // these are values from calib using 2 ohm Rc and Rl 8.6
//int mapE = 650;      // using curr calib sketch; C&D are AR pin values
//int mapF = 880;      // E&F are corresponding measured currents

//int mapC =720;      // empiric C map params, pinIn2 to disch current mA
//int mapD = 978;     // these are values from calib using 2 ohm Rc and Rl 10.5
//int mapE = 700;     // using curr calib sketch; C&D are AR pin values
//int mapF = 900;     // E&F are corresponding measured current

//int mapC =740;      // empiric C map params, pinIn2 to disch current mA
//int mapD = 1000;     // these are values from calib using 2 ohm Rc and Rl 12.5
//int mapE = 700;     // using curr calib sketch; C&D are AR pin values
//int mapF = 900;     // E&F are corresponding measured currents

int mapC = 339;      // empiric C map params, pinIn2 to disch current mA
int mapD = 844;     // these are values from calib using 2 ohm Rc and total Rl 14
int mapE = 400;     // using curr calib sketch; C&D are AR pin values
int mapF = 800;     // E&F are corresponding measured currents

//int mapC = 210;      // empiric Curr map params, pinIn2 to dc current mA
//int mapD = 656;      // these are values from calib using 2.0 ohm Rc and Rl 16
//int mapE = 300;      // using curr calib sketch; C&D are AR pin values
//int mapF = 650;    	 // E&F are corresponding measured currents

//int mapC = 230;      // empiric Curr map params, pinIn2 to dc current mA
//int mapD = 596;      // these are values from calib using 2.0 ohm Rc and Rl 18.5
//int mapE = 300;      // using curr calib sketch; C&D are AR pin values
//int mapF = 560;       // E&F are corresponding measured currents

//int mapC = 109;       // empiric C map params, pinIn2 to dc current mA
//int mapD = 320;       // these are values from calib using 2.0 ohm Rc and Rl 27
//int mapE = 200;       // using curr calib sketch; C&D are AR pin values
//int mapF = 400;       // E&F are corresponding measured currents

//int mapC = 125;      // empiric C map params, pinIn2 to dc current mA
//int mapD = 466;      // these are values from calib using 2.7 ohm Rc and Rl 35
//int mapE = 150;      // using curr calib sketch; C&D are AR pin values
//int mapF = 300;    	 // E&F are corresponding measured currents

//int mapC = 121;      // empiric C map params, pinIn2 to dc current mA
//int mapD = 625;     // these are values from calib using 5 ohm Rc and 36 Rl(adj+2+5 in series)
//int mapE = 80;      // using curr calib sketch; C&D are AR pin values
//int mapF = 250;     // E&F are corresponding measured currents

float newVavg;
float prevVavg;         // voltage globals, used in MA calc in setup calib and loop readout
float indxVavg;

float newCavg;
float prevCavg;         // current globals, used in setup and loop
float indxCavg;

// ************************************

void setup()   // check & calibrate voltage readout, if OK sets starTime for dc readout
{
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(8, 2);

  // first send adjustable voltage to op-amp/ardu, check if readout is accurate
  // if not get new params for map; if OK, exit to readout phase
  int voltage;         // local vars, just used in setup
  int current;
  pinMode(pinIn1, INPUT);  // anal pin to read batt V
  pinMode(pinIn2, INPUT);  // anal pin to read curr sense V
  pinMode(8, OUTPUT);  // toner prn active
  delay(2000);         // wait to get first samples

  prevVavg = analogRead(pinIn1);  // just a starting place for the MA of voltage
  indxVavg = prevVavg;         // start the same, but indxVavg only updates if newVavg
  // is different & prints

  prevCavg = analogRead(pinIn2);
  indxCavg = prevCavg;

  // print to 2 lines of LCD, if it's present
  lcd.clear();  // first clear screen and set the cursor to (0,0)
  lcd.print("start");
  lcd.setCursor(0, 1);  // second line
  lcd.print("record");

  // put calib input (adj V w/ pot) to op-amp and print raw AR and calc V
  // curr map calib using sketch: ReadAnalCurrMap, before doing this
  //  Serial.println("printing pin reading & calcul batt V;");
  //  Serial.println("type x if accurate, m to enter new V map params");
  // x will exit setup, begin recording

  // ************************************

  //  do   // it at least once
  //  {
  //    static byte i = 1;   // so the 'do' prints once at least
  //    readPinAvg();    // gets new_avg of V, C pins over 2 sec., updates globals
  //
  //    // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params
  //    voltage = map(newVavg, mapA, mapB, 11500, 13000); // in mV, resolves ~2 mV
  //    // convert analog reading (0 - 1023) to current via amplified voltage (0-1.9 across Rc)
  //    current = map(newCavg, mapC, mapD, mapE, mapF); // map AR ticks to current (mA) -- drop at Rc
  //
  //    // print a new MA if pinRead changed, reset indexes; or if 1st loop
  //    if ((newVavg != indxVavg) || (i == 1))
  //    {
  //      Serial.print("pin sees= ");
  //      Serial.print(newVavg);
  //      Serial.print(" calc V= ");
  //      Serial.print(voltage);
  //      Serial.print("  ");
  //      Serial.print(current);
  //      Serial.println("  mA");
  //      indxVavg = newVavg;
  //      indxCavg = newCavg;
  //      i++;
  //    }
  //  }  // end do/while no serial input -- you either like the params or want to enter new ones now
  //  while (!Serial.available()); // while no serial input, keep doing the do
  //
  // ************************************

  //  Serial.println("calib done; connect batt, type any letter to start data collection");
  //  delay(50);  //flaky reading of chars unless some delay
  //  while (!Serial.available()); // while, you wait, for batt connection
  // when something typed, wait 5 seconds, get the first batt reading, init starTime
  delay(1000);
  prevVavg = analogRead(pinIn1);
  prevCavg = analogRead(pinIn2);
  for (int i = 0; i < 10; i++) // average for few seconds to stabilize
  {
    int pin1Sees = (analogRead(pinIn1));    // AR the pin sees
    newVavg = (float)pin1Sees * 0.2 + 0.8 * prevVavg; //update expon moving average of AR
    prevVavg = newVavg;

    // read the curr pin, use moving average to stabilize
    int pin2Sees = analogRead(pinIn2);
    newCavg = (float)pin2Sees * 0.2 + 0.8 * prevCavg; // update expon moving average of AR
    prevCavg = newCavg;   //  after loop x 10 goes on from here
    delay(300);
  }  //end for

  // convert analog reading (0 - 1023) to batt voltage (11.5-13) using map params
  voltage = map(newVavg, mapA, mapB, 11500, 13000); // in mV, resolves ~2 mV
  // convert analog reading (0 - 1023) to current (via amplified voltage (0-1.9 across Rc)
  current = map(newCavg, mapC, mapD, mapE, mapF); // map AR ticks to current (mA) for X ohm Rc

  starTime = millis();     // start of data collection, set once
  prevTime = starTime;    // set once here, then reset by each print job
  printVals(voltage, current);   // need starTime defined, now print first time/value set
}    //end setup


/**************************/

void loop()   // read the batt volt, print elapsed time & values, more often as target approached
{
  static boolean done = 0;       // i.e. 0 = not done yet, init to 0 first loop only
  static boolean watch = 0;     // whether to continue plot after target reached, set once
  static boolean pinPrint = 0;  // print pinreads or V/C
  static unsigned long interval = 0; // used in various repeats

  // read the analog input pin
  readPinAvg();  // gets MA x5 of V, C pins, puts it in new_avg global float variables

  // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params
  int battVolt = map(newVavg, mapA, mapB, 11500, 13000); // in mV, resolves ~2 mV
  int battCurr = map(newCavg, mapC, mapD, mapE, mapF); // same for current

  static int baseVolt = battVolt;          // batt V in mV when we started discharging, set once
  static int battRange = (baseVolt - targVolt);  // range from starting high point to target V, set once
  //              Serial.println (battRange);          // should stay fixed
  //              Serial.print("done yet? "); Serial.println(done);

  /**************************/

  while (done != 1) // do this until done is true (target V reached), bypassed when done
  { // repeats q 2 sec because of pinAvg reading delays

    // 2 local vars, only used here, will determine whether to print or wait
    interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
    int deltaV;           // how far present V is above target in mV

    // Serial.print("interval now  "); Serial.println (interval);

    // refresh battVolt 'while looping'
    readPinAvg();  // gets average of V, C pins, puts them in new_avg global variables
    // takes 2 sec.
    battVolt = map(newVavg, mapA, mapB, 11500, 13000); // in mV, resolves ~2 mV
    battCurr = map(newCavg, mapC, mapD, mapE, mapF);
    //  Serial.println("main loop just read battV");
    if (pinPrint == 0)
    { // print V/C to 2 lines of LCD, if it's present, every 2.5 sec, while+readPin delay
      lcd.clear();  // first clear screen and set the cursor to (0,0)
      lcd.print(battVolt);
      lcd.print(" mV");  // the averaged voltage
      lcd.setCursor(0, 1);  // second line
      lcd.print(battCurr);
      lcd.print(" mA");
    }
    else
    { // print raw pin reads to LCD
      lcd.clear();  // first clear screen
      lcd.print("vPin " + String(round(newVavg)));
      lcd.setCursor(0, 1);  // second line
      lcd.print("cPin " + String(round(newCavg)));
    }
    pinPrint = !pinPrint;  // invert value, so LCD alternates showing V/C and pinVals

    deltaV  = (battVolt - targVolt);  //  how far batt is from target now
    //   Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt dc's

    // rounding should give nearest integer to product
    if ( (deltaV > round(battRange * 0.25)) && (interval >= 1 * 60000) )
      printVals(battVolt, battCurr);   //print every 1 min in this range

    if ( (deltaV <= round(battRange * 0.25)) && (deltaV > round(battRange * 0.13)) && (interval >= 0.5 * 60000) )
      printVals(battVolt, battCurr);   //print every 0.5 min

    if ( (deltaV <= round(battRange * 0.13)) && (deltaV > round(battRange * 0.06)) && (interval >= 0.5 * 60000) )
      printVals(battVolt, battCurr);   //print every .5 min

    if ( (deltaV <= round(battRange * 0.06)) && (deltaV >= 1) && (interval >= 0.5 * 60000))
      printVals(battVolt, battCurr);   //print every 0.5 min

    if (deltaV < 1)     // target V reached
    {
      done = 1;        // stops while loop cycling, exits to main loop
      break;
    }
    //    if (abs(prevCavg - newCavg) >= 2)   // also monitor dc current, adjust prn
    //    {
    //      printVals(battVolt,battCurr);
    //    }

    delay(100);      // while, you wait

  }  // end while loop, must be done   // Serial.println("exit while loop");

  /**************************/
  // after the while, main loop does these
  
  interval = (millis() - prevTime);   // ms. elapsed since various things happened

  if (pinPrint == 0)
  { // print V/C to 2 lines of LCD, if it's present, every 2.5 sec, while+readPin delay
    lcd.clear();  // first clear screen and set the cursor to (0,0)
    lcd.print(battVolt);
    lcd.print(" mV");  // the averaged voltage
    lcd.setCursor(0, 1);  // second line
    lcd.print(battCurr);
    lcd.print(" mA");
  }
  else
  { // print raw pin reads to LCD
    lcd.clear();  // first clear screen
    lcd.print("vPin " + String(round(newVavg)));
    lcd.setCursor(0, 1);  // second line
    lcd.print("cPin " + String(round(newCavg)));
  }
  pinPrint = !pinPrint;  // invert value

  if (done == 1 && watch == 0)  // main loop does this block when done is true and not watching
  { // print a fresh value
    // Serial.print("elapsed min.= "), Serial.println((millis() - starTime) / 60000);
    // Serial.print(battVolt);
    Serial.println("discharge target reached");
    delay(3000);
    Serial.println ("for extended dc type e immediately");
    delay(5000);
    if (Serial.read() == 'e')
    {
      Serial.println("values print every 1 min.");
      watch = 1;
    }

    if (watch == 0  && interval > 18000)  // beep if not watching
    {
      for (byte i = 0; i <= 5; i++) // beep x 5 (for 2 sec) every 20 sec
        //      {
        //        tone(8, 2700);   // don't need pin config for toner
        //        delay(500);
        //        noTone(8);
        //        delay(200);
        //      } // end tone
      { // active piezo beeper just needs voltage
        digitalWrite(8, HIGH);   //  need pin config for digiWrite to work
        delay(500);
        digitalWrite(8, LOW);
        delay(200);
      } // end tone loop
      prevTime = millis();
    } // end if no watch
    
  }  //end if done & no watch

  if (done == 1 && watch == 1)  // main loop does this if watch = 1
  {
    interval = (millis() - prevTime);   // ms. elapsed since last printVal job
    // refresh values
    readPinAvg();  // gets average of V, C pins, puts them in new_avg global variables

    int battVolt = map(newVavg, mapA, mapB, 11500, 13000);
    int battCurr = map(newCavg, mapC, mapD, mapE, mapF);

    if (pinPrint == 0)
    { // print V/C to 2 lines of LCD, if it's present, every 2.5 sec, while+readPin delay
      lcd.clear();  // first clear screen and set the cursor to (0,0)
      lcd.print(battVolt);
      lcd.print(" mV");  // the averaged voltage
      lcd.setCursor(0, 1);  // second line
      lcd.print(battCurr);
      lcd.print(" mA");
    }
    else
    { // print raw pin reads to LCD
      lcd.clear();  // first clear screen
      lcd.print("vPin " + String(round(newVavg)));
      lcd.setCursor(0, 1);  // second line
      lcd.print("cPin " + String(round(newCavg)));
    }
    pinPrint = !pinPrint;  // invert value
    if (interval >=  60000) printVals(battVolt, battCurr); // print to ser mon every min
  
  }  // end if done and watching

  delay(200);     // no reason to main loop any faster

}  //end main loop

void readPinAvg()
{ // average 5 pin readings, update globals (new_avg) for C and V
  prevVavg = analogRead(pinIn1);      //just a starting place for the MA of V,C
  prevCavg = analogRead(pinIn2);
  for (int i = 0; i < 5; i++) // get average of 5 readings over ~2 sec.
  {
    int pin1Sees = analogRead(pinIn1);
    newVavg = (float)pin1Sees * 0.2 + 0.8 * prevVavg; // update expon moving average of voltage AR
    prevVavg = newVavg;

    int pin2Sees = analogRead(pinIn2);
    newCavg = (float)pin2Sees * 0.2 + 0.8 * prevCavg; // update expon MA of curr AR
    prevCavg = newCavg;

    delay(400);
  }  // end for
}  // end readPin

// *******************************
// mod for serial output to Proc sketch
// prints elapsed min, mV, mA, resets prevTime to print @ intervals
void printVals(int battV, int curr)
{
  // Serial.print("elapsed min.= ");
  Serial.print((millis() - starTime) / 60000);
  Serial.print(",");
  Serial.print(battV);
  Serial.print(",");
  Serial.println(curr);
  prevTime = millis();
}  //end printVals

//void printVals(int battV, int curr)   //prints batt in mV, current in mA, resets prevTime
//{
//  Serial.print("elapsed min.= ");
//  Serial.print((millis() - starTime) / 60000);
//  //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees);
//  Serial.print("  batt= ");
//  Serial.print(battV);
//  Serial.print(" mV  ");
//  Serial.print("curr= ");
//  Serial.print(curr);
//  Serial.println(" mA");
//  prevTime = millis();
//}  //end printVals

