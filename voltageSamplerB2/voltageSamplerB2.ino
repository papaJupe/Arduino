/* 
VoltSamplerB2  modif from VoltageSimpler12vC, monitors batt discharge, using while loop,
   -- to convert 0-5 v from diff. op-amps to  source batt's ~11-13 v; setup gives calib option to
   enable tune up of map params; loop watches batt volt as it discharges, ser. prints values, more
   as it approaches target.
   Uses: constant, while loop with conditional typed input, calib of map params, expon moving 
   average, Serial.parseInt, while(bool) to pause action
*/

unsigned long starTime;  // starting time for discharge, msec, (global) used in setup and printVals fx
unsigned long prevTime;  // initially = starTime, updated with each printVals, (global, same reason)

     // int analReed;  //  prn to simulate voltage value read from an analog pin
const int targVolt= 12500; // discharge target in mV, varies with batt type, depth you want;
                            // this is 20% dc at C/20 for SLA

#define PinIn1 A5          // pin to read batt voltage
#define PinIn2 A2          // pin to read curr sense voltage

int mapA = 111;          // globals, used in calib and dc readout
int mapB = 965;           // default map params

float newAvg;
float prevAvg;              // 3 globals, used in MA calc in setup calib and dc readout
float compAvg;


void setup()   // check / calibrate voltage readout, if OK sets starTime for dc readout
{  
  Serial.begin(9600);  // first send adjustable voltage to op-amp/ardu, check if readout is accurate
                      // if not get new params for map; if OK, exit to readout phase
  int voltage;         // local var, just used in setup
  delay(3000);         // wait to get first samples
  
  prevAvg = analogRead(PinIn1);  //just a starting place for the MA of voltage
  compAvg = prevAvg;         // starts the same, but this only updates if newAvg is different & prints
         // put calib input to op-amp and print raw AR and calc V until getting serial input x to stop
  Serial.println("printing pin reading & calcul batt V; type x if accurate, m to enter new map params");
         // x will exit setup, begin recording
  while(!Serial.available())  // while no serial input, keep doing this
      {  
        int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
         newAvg = (float)pin1Sees*0.2 + 0.8*prevAvg;  //update expon moving average of AR
        // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params 
        voltage = map(newAvg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
        prevAvg = newAvg;  // updates each loop for expon MA to work
       
       // if MA has changed, print it, reset compAvg
       if (round(newAvg) != round(compAvg))
         {
         Serial.print("pin sees= "); Serial.print(round(newAvg)); Serial.print("  volt calc= "); 
         Serial.println(voltage);
         compAvg = newAvg;
         }
     
      delay (200);   // five while loops / second, takes a while to stabilize
      }  // end while !serial input -- you either like the params or want to enter new ones now
      
    delay(50);  //flaky reading of chars unless some delay

    if  (Serial.read() == 'm')  // you want to enter new map params                              
      { 
       Serial.println("enter new 3 digit # for mapA");
       while(!Serial.available())  // wait for it
       delay(20);  //flaky reading of chars unless some delay

       int mapA = Serial.parseInt();
       Serial.print("new mapA = ");  Serial.print(mapA); Serial.println("   enter new mapB #");
       while(!Serial.available())  // wait for it
       delay(20);  //flaky reading of chars unless some delay

       int mapB = Serial.parseInt();
       Serial.print("new mapB = "); Serial.print(mapB); 
       Serial.println(" -- will show calib readout again:");     
       
         // repeats calib display

         // put calib input to analog pin and print raw AR and calc V until getting serial input to exit
       Serial.println("adjust calib voltages, type x if accurate, \nor put new values in code or reset Ardu to start over");
             // typing x will exit setup, begin recording
       prevAvg = analogRead(PinIn1);  //just some starting place for the MA
       compAvg = prevAvg;
        
       delay(50);  //flaky reading of chars unless some delay
        
       while(Serial.read() != 'x')  // while not x, keep doing this
          {  
           int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
           newAvg = (float)pin1Sees*0.2 + 0.8*prevAvg;  //update expon moving average of AR
            // convert analog reading (0 - 1023) to a voltage (11.5-13) using new map params 
           voltage = map(newAvg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
           prevAvg = newAvg;
           
          // print new Avg if changed, reset comp Avg
          if (newAvg != compAvg)
             {
               Serial.print("pin sees= "); Serial.print(round(newAvg)); 
               Serial.print("  volt calc= "); Serial.println(voltage);
               compAvg = newAvg;
              }
           delay (200);   // five while loops / second
          }  // end while no x input
      }   // end if serial.read 'm'
      
  Serial.println("calib done; connect batt, type any letter to start data collection");
  delay(50);                   //flaky reading of chars unless some delay
  while(!Serial.available())  // while, you wait, for batt connection
          // when something typed, wait 3 seconds, get the first batt reading, init starTime
  delay(3000);
  prevAvg = analogRead(PinIn1);
  for(int i=0;i<10;i++)   // a few seconds to stabilize and get good starting average
        {  int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
        newAvg = (float)pin1Sees*0.2 + 0.8*prevAvg;  //update expon moving average of AR
        // convert analog reading (0 - 1023) to batt voltage (11.5-13) using map params 
        voltage = map(newAvg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
        prevAvg = newAvg;
        delay(300);
        }  //end for
        
  starTime = millis();     // start of data collection, set once
  prevTime = starTime;    // set once here, then reset by each print job
  printVals(voltage);    // need starTime defined, now print first time/value set
}    //end setup

void loop()   // read the batt volt, print out time, value, more often as target approached
{ 
  static boolean done = 0;       // i.e. 0 = not done yet, set first loop only
  
          // read the analog input pin
  prevAvg = analogRead(PinIn1);      //just a starting place for the MA of voltage
  for(int i=0;i<5;i++)                  // average 5 samples
    { 
    int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
    newAvg = (float)pin1Sees*0.2 + 0.8*prevAvg;  //update expon moving average of AR over 1 second
    prevAvg = newAvg;
    delay (200);
    }   
        // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params
  int battVolt = map(newAvg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
          //    Serial.println("main loop just read battV");
  static int baseVolt = battVolt;  // batt V in mV when we started discharging, set once
  static int battRange = (baseVolt - targVolt);  // range from starting high point to target V, set once
          //     Serial.println (battRange);          // should stay fixed
          //     Serial.print("done yet? "); Serial.println(done);
  while(done != 1)       //do this until done is true (target V reached), bypassed when done
   {             
     int interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
     int deltaV;           // how high present V is from target in mV
                           // 2 local vars, only used here, will determine whether to print or wait
      //  Serial.print("interval now  "); Serial.println (interval)
  
      // refresh battVolt 'while looping'
    int prevAvg = analogRead(PinIn1);      //just a starting place for the MA of voltage
    for(int i=0;i<5;i++)    // get average of 5 readings
      {
        int pin1Sees = (analogRead(PinIn1));    // AR the pin sees
         newAvg = (float)pin1Sees*0.2 + 0.8*prevAvg;;  //update expon moving average of AR
              // convert analog reading (0 - 1023) to a voltage (11.5-13) using map params 
        battVolt = map(newAvg,mapA,mapB,11500,13000);  // in mV, resolves ~2 mV
        prevAvg = newAvg;
        delay(200);
      }  // end for
         // Serial.println("main loop just read battV");  
    deltaV  = (battVolt - targVolt);  //  how far batt is from target now
      //   Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt dc's
      //   delay(5000);

    if ( (deltaV > round(battRange * 0.5)) && (interval >= 15 * 60000) ) 
      printVals(battVolt);  //print every 15 min while far above target V
          // rounding should give nearest integer to product
    if ( (deltaV <= round(battRange * 0.5)) && (deltaV > round(battRange * 0.25)) && (interval >= 10 * 60000) )
      printVals(battVolt);    //print every 10 min in this range
      
    if ( (deltaV <= round(battRange * 0.25)) && (deltaV > round(battRange * 0.13)) && (interval >= 5 * 60000) )
      printVals(battVolt);    //print every 5 min
      
    if ( (deltaV <= round(battRange * 0.13)) && (deltaV > round(battRange * 0.06)) && (interval >= 3 * 60000) )
      printVals(battVolt);    //print every 3 min
      
    if ( (deltaV <= round(battRange * 0.06)) && (deltaV >1) && (interval >= 60000))
      printVals(battVolt);    //print every 1 min
      
    if (deltaV <= 1)     // target V reached
      {done = 1;        // stops while loop cycing, exits to main loop
      break;
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

void printVals(int battV)    // prints batt in mV, resets prevTime
{  
       //  int pinSees = (analogRead(battPin));
  Serial.print("elapsed min.= "); Serial.print((millis() - starTime) / 60000);
     //  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees); 
  Serial.print("  batt= "); Serial.print(battV);Serial.println(" mV");
  prevTime = millis();  //tried to print battVolt -- failed to update even when a global
}  //end printVals

