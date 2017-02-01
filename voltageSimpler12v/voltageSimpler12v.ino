/*
voltageSimpler12v -- pre-op-amp attempt to monitor batt discharge, just using while loop;
   mod to read 0-5 v diff of batt v to regulated 8 v. rather than using volt divider
   now calibrates mult first if voltRegul = 0; much improved in later Sampler versions
*/

unsigned long starTime;  // starting time for discharge, msec
unsigned long prevTime;  // init = starTime, updated when each printVal done
unsigned long interval;  //will hold msec. since last printout of Values, big #
float deltaV;       // how high current V is from target in mV
 // int analReed;  //  to simulate voltage value read from an analog pin
const float targVolt= 12500; // discharge target in mV, varies with batt type, depth you want
                            // this is 20% dc at C/20
float voltRegul = 8770;   // possible (?int) value of regulated voltage offset, 13000-voltRegul <= 5K
                          //  full 5 v ~= 1023, gives ~5 mV per tick i.e. sensitivity
                          // set to 0 to calibrate mult initially; should approx= meas v out of VR
float battVolt;            // in mV, calcul from analRead * mult + voltRegul offset; could be int?
#define battPin 3         // input pin for batt volt; need to say A3, no
boolean done = 0;        // not done yet
float mult = 4.875;      // multiplier empirically determined from measured v across VR, may vary betw boards


void setup()   // option to calibrate voltage readout, also sets starTime
{  
  Serial.begin(9600);
  int pinSees = (analogRead(battPin) * mult);   // mV at pin
  battVolt = (analogRead(battPin) * mult + voltRegul);  // take initial reading
  Serial.print("AR x mult="); Serial.print(pinSees); Serial.print(" "); 
  Serial.print("offset is "); Serial.println(voltRegul); 
  Serial.print("calc batt reading = "); Serial.println(battVolt);  Serial.println("need calib.? - type y or n + enter");
  while(!Serial.available());  // while no serial input, hold the action
  if (Serial.available())
   { 
     delay(50);  //flaky reading of chars unless some delay
     if  (Serial.read() == 'y')
      {
          if (voltRegul == 0)  // need to calibr mult
          calMult();
         
         else
         
          calibrate();    // call function or could be offset=calibrate() to call 
         
      }
     else      // any char but y ends the setup; need braces? no
     Serial.println("calib done or bypassed"); //is this enough? yes
    }  //end if serial.avail
  unsigned long starTime = millis();     //start of sampling session, set once
  unsigned long prevTime = starTime;  // set once here, then reset by each print job
  printVals(battVolt);
}    //end setup

void loop()   // read the batt volt, print out time, value, more often as target approached
{ 
    // read the analog input pin

    float battVolt = (analogRead(battPin) * mult + voltRegul);  // float vs. int in mV?
          //    Serial.println("main loop just read battV");
    static float baseVolt = battVolt;  // will be V in mV when we started discharging, set once
    static float battRange = (baseVolt - targVolt);  // range to read from high point to target V
          //     Serial.println (battRange);  // should stay fixed
          //     Serial.print("done yet? "); Serial.println(done);
  while(done != 1)       //do this until done is true, target V reached
   {                    // may not need this while loop but breaks allow quick exit

    interval = (millis() - prevTime);   // ms. elapsed since last printVal job
                              // will determine whether to print or wait
      //   Serial.print("int now  "); Serial.println (interval);
    float battVolt = (analogRead(battPin) * mult + voltRegul);  // keep refreshed battVolt while looping here
    float deltaV  = (battVolt-targVolt);  //how far batt is from target now
      //   Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt decays
      //   delay(5000);

    if ( (deltaV > battRange * 0.5) && (interval >= 15 * 60000) ) 
      {printVals(battVolt);  //we print every 15 min while far above target V
      break;    // exits while loop to main loop, to block evaluating later if's
      }
    if ( (deltaV <= battRange * 0.5) && (deltaV > battRange * 0.25) && (interval >= 10 * 60000) )
      {printVals(battVolt);       //print every 10 min
      break;
      }
    if ( (deltaV <= battRange * 0.25) && (deltaV > battRange * 0.13) && (interval >= 5 * 60000) )
      {printVals(battVolt);    //print every 5 min
      break;
      }
    if ( (deltaV <= battRange * 0.13) && (deltaV > battRange * 0.06) && (interval >= 3 * 60000) )
      {printVals(battVolt);    //print every 3 min
      break;
      }
    if ( (deltaV <= battRange * 0.06) && (deltaV >1) && (interval >= 60000) )
      {printVals(battVolt);    //print every 1 min
      break;
      }
    if (deltaV <= 1)     // target V reached
      {done = 1;        // stops this while loop from repeating
      break;
      }
          // need way to exit while loop if none of above are true? no,
         // not if variables to evaluate are inside this loop
     delay(1000);      // while, you wait
   }  // end while loop
   
//   Serial.println("exit while loop");

  delay(1000);     // no reason to loop any faster
  
  if (done == 1)  // main loop does this block repeatedly when done
   {
      printVals(battVolt);  Serial.println("  -- discharge target reached");  
      Serial.println ("NOW - STOP ME WHILE YOU CAN!");
      for (byte i=0; i<=6; i++)  // play finish tone every 15 sec
        {
         tone(8, 2700);   // don't need pin config for toner
         delay(500);
         noTone(8);
         delay(200);
        } // end tone loop
     delay (15000);
   }  //end terminal if
}  //end main loop

void calMult() //could, but doesn't actually return value, just resets global mult
 {
  // reads the analog pin, adjust mult to read correct voltage, i.e. matching external v.meter
  // looking at v. to analog pin 3 vs. board grnd
  Serial.println("type u to raise V, d to lower, x to exit");
  char ch;
  while(!Serial.available());  // no serial input, hold the action
  while(ch != 'x')   //loop until x typed
   {  
     delay(100);
     ch = Serial.read();
     if(ch =='u')
     {
      mult = (mult + 0.005);
      battVolt = (analogRead(battPin) * mult);
     Serial.print("mult is now "); Serial.print(mult); Serial.print(" - volts read "); 
     Serial.println(battVolt);
     }
     else if(ch =='d')
     {
      mult = (mult - 0.003);
      battVolt = (analogRead(battPin) * mult);
     Serial.print("mult is now "); Serial.print(mult); Serial.print(" - volts read "); 
     Serial.println(battVolt);
     }  //end else
     delay(100);
    }  // end while, presumably only after an x is typed
  Serial.println("calib. done, starting record");
      //     nothing returns ; mult global has been reset by function
}   //end calibMult function

float calibrate() //could, but doesn't actually return value, just resets a global
 {
  // read the analog pin, adjust voltRegul to read correct voltage, i.e. matching external v.meter
  Serial.println("type u to raise V, d to lower, x to exit");
  char ch;
  while(!Serial.available());  // no serial input, hold the action
  while(ch != 'x')   //loop until x typed
   {  
     delay(100);
     ch = Serial.read();
     if(ch =='u')
     {
      voltRegul = (voltRegul + 5);
      battVolt = (analogRead(battPin) * mult + voltRegul);
     Serial.print("offset is now "); Serial.print(voltRegul); Serial.print(" - volts read "); 
     Serial.println(battVolt);
     }
     else if(ch =='d')
     {
      voltRegul = (voltRegul - 4);
      battVolt = (analogRead(battPin) * mult + voltRegul);
     Serial.print("offset is now "); Serial.print(voltRegul); Serial.print(" - volts read "); 
     Serial.println(battVolt);
     }  //end else
     delay(100);
    }  // end while, presumably only after an x is typed
    Serial.println("calib. done, starting record");
//    return offset;  //not necessary because voltRegul global has been reset by function
}   //end calib function
 
void printVals(int battV)    //prints current mV as integer, resets prevTime
{  
  int pinSees = (analogRead(battPin) * mult);
  Serial.print("elapsed min.= ");Serial.print((millis() - starTime) / 60000);
  Serial.print(" "); Serial.print("AR x mult= "); Serial.print(pinSees); Serial.print(" "); 
  Serial.print("batt= "); Serial.print(battV);Serial.println(" mV");
  prevTime = millis();  //tried print battVolt, failed to update even tho a global
}  //end printVals
