/* 
 VoltageSamplerDisch, modif of VoltageSamplerB4, made to roughly monitor discharge, 
 using external R, to 12.4 v for conditioning; drops curr monitor, etc. 
 
 */

unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals fx
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global, same reason)

// int analReed;   to simulate voltage value read from an analog pin
const int targVolt= 12400; // discharge target in mV, this is ~33% dc at C/20 for SLA; used only in loop, init here for clarity
// for 8x2 LCD, include the library
#include <LiquidCrystal.h>
// initialize new lcd object with the Arduino pins
LiquidCrystal lcd(12,11,4,5,6,7);  // lib is smart enough to know if you're using 4 pins or 8

#define pinIn1 A1          // pin to read test batt voltage

int mapA = 132;           // default V map params, A for 11.5, B for 13.0
int mapB = 963;           // globals, used in calib and dc readout

int newVavg;
int prevVavg;              // 3 V globals, used in MA calc in setup calib and dc readout
int indxVavg;

// ************************************

void setup()   // check & calibrate voltage readout, if OK sets starTime for dc readout
{  
  Serial.begin(9600);  
  // set up the LCD's number of columns and rows: 
  lcd.begin(8,2);

  // first send adjustable voltage to op-amp/ardu, check if readout is accurate
  // if not get new params for map; if OK, exit to readout phase
  int voltage;         // local vars, just used in setup
  pinMode(pinIn1, INPUT);

  delay(3000);         // wait to get first samples

  prevVavg = analogRead(pinIn1);  //just a starting place for the MA of voltage
  indxVavg = 1025;         // higher than it ever could be, so we get something to print quickly

  // refresh battVolt 'while looping'
  readPinAvg();  // gets average of V pins, puts them in newVavg global variable

  int battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV


  // print to 2 lines of LCD, if it's present, once only
  lcd.clear();  // first clear screen and set the cursor to (0,0)       
  lcd.print(battVolt);
  lcd.print(" mV");  // the averaged voltage 


  // put calib input (adj V w/ pot) to op-amp and print raw AR and calc V until getting serial input x to stop
  // curr map calib using that sketch, AnalCurrMap ?
  Serial.println("printing pin reading & calcul batt V;");
  Serial.println("type x to start recording");
  // x will exit setup, begin recording

  // ************************************

  do   // it at least once
  { 
    readPinAvg();    // gets avg of V pins over 1 sec., updates globals

    // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params 
    voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
    // print a new MA if pinRead changed, reset indexes
    if (newVavg != indxVavg)
    {
      Serial.print("pin sees= "); 
      Serial.print(newVavg); 
      Serial.print(" calc V= "); 
      Serial.print(voltage);
      lcd.clear();  // first clear screen and set the cursor to (0,0)       
      lcd.print(voltage);
      lcd.print(" mV");  // the averaged voltage 

      indxVavg = newVavg;
    }  // end if
  }  // end do/while no serial input -- you either like the params or want to enter new ones now
  while(!Serial.available());  // while no serial input, keep doing the do

    delay(50);  //flaky reading of chars unless some delay
        // this while maybe not needed 
    while(Serial.read() != 'x')  // while not x, keep doing this, just checking V
    {  
      int pin1Sees = (analogRead(pinIn1));    // AR the pin sees
      newVavg = (pin1Sees + 4*prevVavg)/5;  //update expon moving average of AR
      // convert analog reading (0 - 1023) to a voltage (11.5-13) using new map params 
      voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
      prevVavg = newVavg;

      // print new Avg if changed, reset V index

      if (newVavg != indxVavg)
      {
        Serial.print("pin sees= "); 
        Serial.print(newVavg); 
        Serial.print("  volt calc= "); 
        Serial.println(voltage);
        indxVavg = newVavg;
      }
      delay (200);   // five while loops / second
    }  // end while no x input
 
  // ************************************

  delay(2000);
  prevVavg = analogRead(pinIn1);
  for(int i=0;i<10;i++)   // average for few seconds to stabilize
  {   
    int pin1Sees = (analogRead(pinIn1));    // AR the pin sees
    newVavg = round((pin1Sees + 4*prevVavg)/5);  //update expon moving average of AR
    prevVavg = newVavg;
    delay(300);
  }  //end for

  // convert analog reading (0 - 1023) to batt voltage (11.5-13) using map params 
  voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV    

  starTime = millis();     // start of data collection, set once
  prevTime = starTime;    // set once here, then reset by each print job
  printVals(voltage);    // need starTime defined, now print first time/value set
  
}    //end setup

void loop()   // read the batt volt, print out time, values, more often as target approached
{ 
  static boolean done = 0;       // i.e. 0 = not done yet, init to 0 first loop only
  static boolean watch = 0;        // whether to continue plot after target reached
  // read the analog input pin
  readPinAvg();  // gets average of V pins, puts it in newXavg global variable

  // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params
  int battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV

  static int baseVolt = battVolt;          // batt V in mV when we started discharging, set once
  static int battRange = (baseVolt - targVolt);  // range from starting high point to target V, set once
  //              Serial.println (battRange);          // should stay fixed
  //              Serial.print("done yet? "); Serial.println(done);
  while(done != 1)  // do this until done is true (target V reached), bypassed when done
  {             
    unsigned long interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
    int deltaV;           // how high present V is from target in mV
    // 2 local vars, only used here, will determine whether to print or wait
    // Serial.print("interval now  "); Serial.println (interval);

    // refresh battVolt 'while looping'
    readPinAvg();  // gets average V, puts in newXavg global variable

    battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV


    // print to 2 lines of LCD, if it's present, every 1 sec, while loop delay
    lcd.clear();  // first clear screen and set the cursor to (0,0)       
    lcd.print(battVolt);
    lcd.print(" mV");  // the averaged voltage 

    deltaV  = (battVolt - targVolt);  //  how far batt is from target now
    //   Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt dc's

    if ( (deltaV > round(battRange * 0.5)) && (interval >= 15 * 60000) ) 
      printVals(battVolt);  //print every 15 min while far above target V
    // rounding should give nearest integer to product
    if ( (deltaV <= round(battRange * 0.5)) && (deltaV > round(battRange * 0.25)) && (interval >= 10 * 60000) )
      printVals(battVolt);    //print every 10 min in this range

    if ( (deltaV <= round(battRange * 0.25)) && (deltaV > round(battRange * 0.13)) && (interval >= 5 * 60000) )
      printVals(battVolt);    //print every 5 min

    if ( (deltaV <= round(battRange * 0.13)) && (deltaV > round(battRange * 0.06)) && (interval >= 3 * 60000) )
      printVals(battVolt);    //print every 3 min

    if ( (deltaV <= round(battRange * 0.06)) && (deltaV >=1) && (interval >= 60000))
      printVals(battVolt);    //print every 1 min

    if (deltaV < 1)     // target V reached
    {
      done = 1;        // stops while loop cycing, exits to main loop
      break;
    }
    delay(1000);      // while, you wait
  }  // end while loop, must be done   // Serial.println("exit while loop")

  // print to 2 lines of LCD, if it's present every 2 sec, main loop delay
  lcd.clear();  // first clear screen and set the cursor to (0,0)       
  lcd.print(battVolt), lcd.print(" mV");  // the averaged voltage 

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

  if (done == 1 && watch == 1)  // main loops this if watch = 1
  {
    unsigned long interval = (millis() - prevTime);   // ms. elapsed since last printVal job
    // refresh values
    readPinAvg();  // gets average of V pin, puts in newXavg global variable

    int battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV 
    if (interval >= 3 * 60000) printVals(battVolt);  // print to ser mon every 3 min
  }  //end if done and watch

  delay(2000);     // no reason to main loop any faster

}  //end main loop

void readPinAvg()
{   // average 5 pin readings, update globals
  prevVavg = analogRead(pinIn1);      //just a starting place for the MA of V
  for(int i=0;i<5;i++)    // get average of 5 readings over 2 sec.
  {
    int pin1Sees = analogRead(pinIn1);   
    newVavg = round((pin1Sees + 4*prevVavg)/5);  //update expon moving average of AR
    prevVavg = newVavg;  

    delay(400);
  }  // end for
}  // end readPin

// *******************************
void printVals(int battV)    // Serial prints batt in mV, resets prevTime
{  
  Serial.print("elapsed min.= "); 
  Serial.print((millis() - starTime) / 60000);
  //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees); 
  Serial.print("  batt= "); 
  Serial.print(battV);
  Serial.print(" mV");
  prevTime = millis(); 
}  //end printVals






