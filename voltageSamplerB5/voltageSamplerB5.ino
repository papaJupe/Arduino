///*
//voltageSamplerB5 -- modif from VoltageSamplerB4: just reads batt V, record deeper disch;
//   -- converts 0-5 v from diff. op-amps to actual 11.5-13 v from batt source using map;
//    calib phase for V in setup; no Curr monitoring here;
// */

unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals fx
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global, same reason)

const int targVolt= 12350; // discharge target in mV, varies with batt type, depth you want;
                   // this is arbitr target for 12aH SLA; used only in loop, init here for clarity

// for 8x2 LCD, include the library
#include <LiquidCrystal.h>
// initialize new lcd object with the Arduino pins it uses
LiquidCrystal lcd(12,11,4,5,6,7);

#define PinIn1 A5          // pin to read test batt voltage

int mapA = 119;            // default V map params
int mapB = 958;            // globals, used in calib and dc readout

int newVavg;
int prevVavg;              // 3 voltage globals, used in MA calc in setup calib and dc readout
int indxVavg;

   // ************************************

void setup()   // check & calibrate voltage readout, if OK sets starTime for dc readout
{    // can first send adjustable voltage to op-amp/ardu, check if readout is accurate
       // if not get new params for map; if OK, exit to readout phase
  Serial.begin(9600);
        // set up the LCD's number of columns and rows: 
  lcd.begin(8,2);
  
  int voltage;         // local vars, just used in setup
  delay(3000);         // wait to get first samples
  
  prevVavg = analogRead(PinIn1);  // just a starting place for the MA of voltage
  indxVavg = prevVavg;     // starts the same, but only updates if newVavg is different
  Serial.println("printing pin reading & calcul batt V;");
  Serial.println("type x if accurate, m to enter new V map params");
         // x will exit setup, begin recording
         
          // print to 2 lines of LCD, if it's present
  lcd.clear();  // first clear screen and set the cursor to (0,0)       
  lcd.print("calib w/"); 
  lcd.setCursor(0,1);   // second line
  lcd.print("ser mon");
         
  // ************************************
  
  do   // it at least once
     { 
        static byte i = 1;  // to print first values
        readPinAvg();    //gets avg of V pins over 2 sec., updates globals
           
       // print a new MA if pinRead changed or 1st loop, reset indexes
       if ((newVavg != indxVavg) || (i == 1))
         {
              // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params 
         voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
        
         Serial.print("pin sees= "); Serial.print(newVavg); Serial.print(" calc V= ");
         // Serial.print("pin sees= " + String(newVavg) +" calc V= " + String(voltage) + "\n");
         Serial.println(voltage); // try above simpler version
         indxVavg = newVavg; i++;
         }
     }  // end do/while no serial input -- you either like the params or want to enter new ones now
    while(!Serial.available());  // while no serial input, keep doing the do

   // ************************************

    delay(50);  // flaky reading of chars unless some delay

    if  (Serial.read() == 'm')  // you want to enter new map params                              
      { 
       Serial.println("enter new 3 digit # for mapA");
       while(!Serial.available());  // wait for it
       delay(50);  //flaky reading of chars unless some delay

       int mapA = Serial.parseInt();
       Serial.print("new mapA = ");  Serial.print(mapA); Serial.println("  enter new mapB #");
       while(!Serial.available());  // wait for it
       delay(50);  //flaky reading of chars unless some delay

       int mapB = Serial.parseInt();
       Serial.print("new mapB = "); Serial.print(mapB); 
       Serial.println(" -- will show calib readout again:");     
       
         // repeats V calib display

         // put calib input to analog pin and print raw AR and calc V until getting serial input to exit
       Serial.println("adjust calib voltages, type x if accurate, reset Ardu to start over");
             // x will exit setup, begin recording
        
       delay(50);  // flaky reading of chars unless some delay
        
       while(Serial.read() != 'x')  // while not x, keep doing this, just checking V
          {  
             static byte j = 1;  // so it prints at least once
           
//           int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
//           newVavg = (pin1Sees + 4*prevVavg)/5;  //update expon moving average of AR
//            // convert analog reading (0 - 1023) to a voltage (11.5-13) using new map params 
//           voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
//           prevVavg = newVavg;
                // just let the pinRead fx do it
            readPinAvg();   // gets a newVavg of 5 readings over 2 sec
           
          // print voltage equiv of newVavg if changed from last, or it's first loop
          // reset indxVavg
          if ((newVavg != indxVavg) || (j == 1))
             {
               voltage = map(newVavg,mapA,mapB,11500,13000);
               Serial.print("pin sees= "); Serial.print(newVavg); 
               Serial.print("  volt calc= "); Serial.println(voltage);
               indxVavg = newVavg; j++;
              }
           delay (200);
          }  // end while no x input
      }   //end if serial.read 'm'
   // ************************************
   
  Serial.println("calib done; connect batt; type any letter to start data collection");
  delay(50);  // flaky reading of chars unless some delay
  while(!Serial.available());  // while, you wait, for batt connection
          // when something typed, wait 5 seconds, get the first batt reading, init starTime
  delay(5000);
  prevVavg = analogRead(PinIn1);
  for(int i=0;i<10;i++)   // average for few seconds to stabilize
      {   
        int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
        newVavg = (pin1Sees + 4*prevVavg)/5;  //update expon moving average of AR
        prevVavg = newVavg;
        delay(300);
        }  //end for
    // convert analog reading (0 - 1023) to batt voltage (11.5-13) using map params 
  voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
        
  starTime = millis();            // start of data collection, set once
  prevTime = starTime;        // set once here, then reset by each print job
  printVals(voltage);
}    //end setup

void loop()   // read the batt volt, print out time, value, more often as target approached
{ 
  static boolean done = 0;       // i.e. 0 = not done yet, init to 0 first loop only
  
          // read the analog input pin
  readPinAvg();  // gets average of V, C pins, puts it in newXavg global variable
    
        // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params
  int battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
  static int baseVolt = battVolt;          // batt V in mV when we started discharging, set once
  static int battRange = (baseVolt - targVolt);  // range from starting high point to target V, set once
    //      Serial.println (battRange);          // should stay fixed
    //      Serial.print("done yet? "); Serial.println(done);
    
  while(done != 1)       //do this until done is true (target V reached), bypassed when done
   {             
     unsigned long interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
     int deltaV;           // how high present V is from target in mV
                           // 2 local vars, only used here, will determine whether to print or wait
      //  Serial.print("interval now  "); Serial.println (interval);
  
      // refresh battVolt 'while looping'
    readPinAvg();  // gets average of V pin => newVavg global variable

    battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
     //   Serial.println("main loop just read battV");

    // print to 2 lines of LCD, if it's present, every 2.5 sec, while + pinRead delay
    lcd.clear();  // first clear screen and set the cursor to (0,0)       
    lcd.print(battVolt);
    lcd.print(" mV");  // the averaged voltage
    lcd.setCursor(0,1); lcd.print((millis() - starTime) / 60000); lcd.print(" min");
    
    deltaV  = (battVolt - targVolt);  //  how far batt is from target now
//     Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt dc's
    
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
        done = 1;        // stops while loop cycling, exits to main loop
        break;
      }

     delay(500);      // while, you wait
   }  // end while loop, must be done
       // need way to exit while loop if none of above are true? no, remove source or reset
         // Serial.println("exit while loop");
         
   // print to 2 lines of LCD, if it's present every 2 sec (main loop delay)
  // so LCD shows action when done is true
  lcd.clear();  // first clear screen and set the cursor to (0,0)       
  lcd.print(battVolt);
  lcd.print(" mV");  // the averaged voltage; show elapsed min on 2nd line:
  lcd.setCursor(0,1); lcd.print((millis() - starTime) / 60000); lcd.print(" min");
  
  if (done == 1)  // main loop does this block repeatedly when done is true
   {       // print a fresh value
      Serial.print(battVolt); Serial.println("  -- discharge target reached");  
      Serial.println ("NOW - STOP ME WHILE YOU CAN!");
      for (byte i=0; i<=6; i++)  // play finish tone every 20 sec
        {
         tone(8, 2700);   // don't need pin config for toner
         delay(500);
         noTone(8);
         delay(200);
        } // end tone loop
     delay (18000);   // 2 + 18 = 20 sec between plays
   }  //end terminal if

delay(2000);     // no reason to main loop any faster
  
}  //end main loop

 // *******************************
 
void readPinAvg()
  {   // average 5 pin readings over 2 sec., update globals
    prevVavg = analogRead(PinIn1);  //just a starting place for the MA of V
    for(int i=0;i<5;i++)    // get average of 5 readings over 2 sec.
      {
        int pin1Sees = analogRead(PinIn1);   
        newVavg = round((pin1Sees + 4*prevVavg)/5);  //update expon moving average of AR
        prevVavg = newVavg;   // prevV used by setup and here, as local var
        delay(400);
       }  // end for
  }  // end readPin
  
 // *******************************

void printVals(int battV)   // prints batt in mV, resets prevTime
{  
  Serial.print("elapsed min.= "); Serial.print((millis() - starTime) / 60000);
     //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees); 
  Serial.print("  batt= "); Serial.print(battV); Serial.println(" mV");
  prevTime = millis(); 
}  //end printVals just for V
