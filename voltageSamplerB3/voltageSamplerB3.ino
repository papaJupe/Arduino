/*
voltageSamplerB3 -- modif from VoltageSamplerB2, adds curr readout
    and other refinements, do/while loop, function for value reading;
   -- converts 0-5 v from diff. op-amps to actual ~11.5-13 v from batt source
    and Rcurr drop to current draw; calib phase for V in setup; C params
    come from previous calib
 */

unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals fx
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global, same reason)

     // int analReed;   to simulate voltage value read from an analog pin
const int targVolt= 12500; // discharge target in mV, varies with batt type, depth you want;
                   // this is 20% dc at C/20 for SLA; used only in loop, init here for clarity

#define PinIn1 3          // pin to read test batt voltage
#define PinIn2 5          // pin to read curr sense voltage

int mapA = 122;            // default V map params
int mapB = 961;            // globals, used in calib and dc readout
int mapC = 138;           // default C map params, pinIn2 to dc current mA
int mapD = 982;           // these are values from calib using 2 ohm Rc
int mapE = 230;           // C & D are raw AR pin values, E & F are corresponding mA values
int mapF = 900;           // they map to; get these from ReadAnalCurrMap ?

int newVavg;
int prevVavg;              // 3 voltage globals, used in MA calc in setup calib and dc readout
int indxVavg;

int newCavg;
int prevCavg;              // 3 current globals, used in setup and loop MAs
int indxCavg;

   // ************************************

void setup()   // check & calibrate voltage readout, if OK sets starTime for dc readout
{  
  Serial.begin(9600);  // first send adjustable voltage to op-amp/ardu, check if readout is accurate
                      // if not get new params for map; if OK, exit to readout phase
  int voltage;         // local vars, just used in setup
  int current;
  delay(3000);         // wait to get first samples
  
  prevVavg = analogRead(PinIn1);  //just a starting place for the MA of voltage
  indxVavg = prevVavg;     // starts the same, but only updates if newVavg is different & prints
  prevCavg = analogRead(PinIn2);
  indxCavg = prevCavg;
         // put calib input to op-amp and print raw AR and calc V until getting serial input x to stop
  Serial.println("printing pin reading & calcul batt V;");
  Serial.println("type x if accurate, m to enter new V map params");
         // x will exit setup, begin recording
         
  // ************************************
  
  do   // it at least once
     { 
        readPinAvg();    //gets avg of V,C pins over 1 sec., updates globals
           
         // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params 
        voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
         // convert analog reading (0 - 1023) to current via amplified voltage (0-1.9 across Rc) 
        current = map(newCavg,mapC,mapD,mapE,mapF);  // map AR ticks to current (mA) for 2 ohm Rc
        
       // print a new MA if pinRead changed, reset indexes
       if (newVavg != indxVavg)
         {
         Serial.print("pin sees= "); Serial.print(newVavg); Serial.print(" calc V= "); Serial.print(voltage);
         Serial.print("  ");Serial.print(current); Serial.println("   mA");
         indxVavg = newVavg;
         indxCavg = newCavg;
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
       Serial.print("new mapA = ");  Serial.print(mapA); Serial.println("   enter new mapB #");
       while(!Serial.available());  // wait for it
       delay(50);  //flaky reading of chars unless some delay

       int mapB = Serial.parseInt();
       Serial.print("new mapB = "); Serial.print(mapB); 
       Serial.println(" -- will show calib readout again:");     
       
         // repeats V calib display

         // put calib input to analog pin and print raw AR and calc V until getting serial input to exit
       Serial.println("adjust calib voltages, type x if accurate, reset Ardu to start over");
             // x will exit setup, begin recording
             
             // dont need to reinit these ?
//       prevVavg = analogRead(PinIn1);  //just some starting place for the MA
//       indxVavg = prevVavg;
        
       delay(50);  // flaky reading of chars unless some delay
        
       while(Serial.read() != 'x')  // while not x, keep doing this, just checking V
          {  
           int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
           newVavg = (pin1Sees + 4*prevVavg)/5;  //update expon moving average of AR
            // convert analog reading (0 - 1023) to a voltage (11.5-13) using new map params 
           voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
           prevVavg = newVavg;
           
          // print new Avg if changed, reset V index
          if (newVavg != indxVavg)
             {
               Serial.print("pin sees= "); Serial.print(newVavg); Serial.print("  volt calc= "); Serial.println(voltage);
               indxVavg = newVavg;
              }
           delay (200);   // five while loops / second
          }  // end while no x input
      }   //end if serial.read 'm'
   // ************************************
   
  Serial.println("calib done; connect batt, type any letter to start data collection");
  delay(50);  // flaky reading of chars unless some delay
  while(!Serial.available());  // while, you wait, for batt connection
          // when something typed, wait 5 seconds, get the first batt reading, init starTime
  delay(5000);
  prevVavg = analogRead(PinIn1);
  prevCavg = analogRead(PinIn2);
  for(int i=0;i<10;i++)   // average for few seconds to stabilize
      {   
        int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
        newVavg = (pin1Sees + 4*prevVavg)/5;  //update expon moving average of AR
        prevVavg = newVavg;
        
         // read the curr pin, make moving average to stabilize 
        int pin5Sees = analogRead(PinIn2);
        newCavg = (pin5Sees + 4*prevCavg)/5;  //update expon moving average of AR
        prevCavg = newCavg;   //stop here
        delay(300);
        }  //end for
            // convert analog reading (0 - 1023) to batt voltage (11.5-13) using map params 
  voltage = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
           // convert analog reading (0 - 1023) to current (via amplified voltage (0-1.9 across Rc) 
  current = map(newCavg,mapC,mapD,mapE,mapF);  // map AR ticks to current (mA) for 2 ohm Rc     
        
  starTime = millis();            // start of data collection, set once
  prevTime = starTime;        // set once here, then reset by each print job
  printVals(voltage,current);    // need starTime defined, now print first time/value set
}    //end setup

void loop()   // read the batt volt, print out time, value, more often as target approached
{ 
  static boolean done = 0;       // i.e. 0 = not done yet, init to 0 first loop only
  
          // read the analog input pin
  readPinAvg();  // gets average of V, C pins, puts it in newXavg global variable
    
        // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params
  int battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
  int battCurr = map(newCavg,mapC,mapD,mapE,mapF);  //same for current

  static int baseVolt = battVolt;          // batt V in mV when we started discharging, set once
  static int battRange = (baseVolt - targVolt);  // range from starting high point to target V, set once
    //              Serial.println (battRange);          // should stay fixed
    //              Serial.print("done yet? "); Serial.println(done);
  while(done != 1)       //do this until done is true (target V reached), bypassed when done
   {             
     unsigned long interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
     int deltaV;           // how high present V is from target in mV
                           // 2 local vars, only used here, will determine whether to print or wait
      //  Serial.print("interval now  "); Serial.println (interval);
  
      // refresh battVolt 'while looping'
    readPinAvg();  // gets average of V, C pins, puts it in newXavg global variable

    battVolt = map(newVavg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
    battCurr = map(newCavg,mapC,mapD,mapE,mapF);
//          Serial.println("main loop just read battV");
         
    deltaV  = (battVolt - targVolt);  //  how far batt is from target now
//         Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt dc's
    
    if ( (deltaV > round(battRange * 0.5)) && (interval >= 15 * 60000) ) 
      printVals(battVolt,battCurr);  //print every 15 min while far above target V
          // rounding should give nearest integer to product
    if ( (deltaV <= round(battRange * 0.5)) && (deltaV > round(battRange * 0.25)) && (interval >= 10 * 60000) )
      printVals(battVolt,battCurr);    //print every 10 min in this range
      
    if ( (deltaV <= round(battRange * 0.25)) && (deltaV > round(battRange * 0.13)) && (interval >= 5 * 60000) )
      printVals(battVolt,battCurr);    //print every 5 min
      
    if ( (deltaV <= round(battRange * 0.13)) && (deltaV > round(battRange * 0.06)) && (interval >= 3 * 60000) )
      printVals(battVolt,battCurr);    //print every 3 min
      
    if ( (deltaV <= round(battRange * 0.06)) && (deltaV >=1) && (interval >= 60000))
      printVals(battVolt,battCurr);    //print every 1 min
      
    if (deltaV < 1)     // target V reached
      {done = 1;        // stops while loop cycing, exits to main loop
      break;
      }
    if (abs(prevCavg - newCavg) >= 2)   // also monitor dc current, adjust
       {
       printVals(battVolt,battCurr);
       }
     delay(2000);      // while, you wait
   }  // end while loop, must be done
           // need way to exit while loop if none of above are true? no, remove source or reset
           // Serial.println("exit while loop");
  
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

void readPinAvg()
  {   // average 5 pin readings, update globals
    prevVavg = analogRead(PinIn1);      //just a starting place for the MA of V,C
    prevCavg = analogRead(PinIn2);
    for(int i=0;i<5;i++)    // get average of 5 readings over 1 sec.
      {
        int pin1Sees = analogRead(PinIn1);   
        newVavg = round((pin1Sees + 4*prevVavg)/5);  //update expon moving average of AR
        prevVavg = newVavg;
        
        int pin2Sees = analogRead(PinIn2);
        newCavg = round((pin2Sees + 4*prevCavg)/5);  //update expon MA of curr AR over 1 second
        prevCavg = newCavg;  
        
        delay(200);
       }  // end for
  }  // end readPin
  
 // *******************************
void printVals(int battV,int curr)    //prints batt in mV, current in mA, resets prevTime
{  
  Serial.print("elapsed min.= "); Serial.print((millis() - starTime) / 60000);
     //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees); 
  Serial.print("  batt= "); Serial.print(battV);Serial.print(" mV  ");
  Serial.print("curr= "); Serial.print(curr);Serial.println(" mA");
  prevTime = millis(); 
}  //end printVals
