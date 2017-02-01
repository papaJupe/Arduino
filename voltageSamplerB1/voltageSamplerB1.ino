/* 
voltSamplerB1 -- monitor batt discharge, modif from VoltageSimpler12vC
	using while loop to check calib accuracy of measured voltage, then go
	to live recording of voltage vs. time, printing as needed. Calculations
	convert 0-5 v from diff. op-amps to actual ~11-13 v batt under test
	to get better accuracy than 5 mv / tick; determine bOffset from
	voltmeter on ref. volt, mult by trial and error?; no live update of params in code
	Uses: while loop with keybd input conditions, while(bool) to hold action 
*/

unsigned long starTime;  // starting time for discharge, msec
unsigned long prevTime;  // init = starTime, updated when each printVal done
unsigned long interval;  //will hold msec. since last printout of Values, big #
float deltaV;           // how high present V is from target in mV
 // int analReed;  //  to simulate voltage value read from an analog pin
const float targVolt= 12500; // discharge target in mV, varies with batt type, depth you want;
                            // this is 20% dc at C/20

  // conversion params from external calibration
float bOffset = 11568;   //  b offset from previous graphed calcul using 3404B circuit values
float mult = 1.75;       //  M multiplier (slope) from prev graphed calcul with 3404B
float battVolt;         // in mV, calcul from analRead * mult + bOffset; could be int?
#define battPin 3       // input pin for batt volt; need to say A3, no
boolean done = 0;       // i.e. 0=not done yet


void setup()   // check / calibrate voltage readout, also sets starTime once acceptable
{  
  Serial.begin(9600);
  int pinSees = (analogRead(battPin));    // just what the pin sees
  battVolt = (pinSees * mult + bOffset);  // take initial reading
  Serial.print("raw AnalRead="); Serial.println(pinSees); 
  Serial.print("mult is "); Serial.print(mult); Serial.print("; offset is "); Serial.println(bOffset); 
  Serial.print("calc batt reading = "); Serial.println(battVolt); 
  Serial.println("need more calib.? N - connect test batt, type n+enter; Y - change source and type y+enter");
  while(!Serial.available())  // while no serial input, hold the action here
  while(Serial.available() !='n')  //repeat until n is typed to stop calib
                                   // shouldn't loop here at all if n typed above
	   { 
		 delay(50);  //flaky reading of chars unless some delay
		 if  (Serial.read() == 'y')  //read and printVals again once
		  {
			int pinSees = (analogRead(battPin));    // updates what the pin sees
			battVolt = (pinSees * mult + bOffset);  // take another reading
			Serial.print("raw AR="); Serial.println(pinSees); 
			Serial.print("mult is "); Serial.print(mult); Serial.print("; offset is "); Serial.println(bOffset); 
			Serial.print("calc batt reading = "); Serial.println(battVolt); 
			Serial.println("need more calib.? Y -- change source and type y+enter");
			Serial.println("if N -- connect test batt and type n+enter");     
		  }  //end if for serial read = Y, repeats
	   }   //end while, stop calib readings
  Serial.println("calib done; starting data collection"); //is this enough? yes
  unsigned long starTime = millis();     //start of sampling session, set once
  unsigned long prevTime = starTime;  // set once here, then reset by each print job
  printVals(battVolt);   // need starTime defined to print first value set
}    //end setup

void loop()   // read the batt volt, print out time, value, more often as target approached
{ 
          // read the analog input pin; rounded int OK if using mV, need cast after mult?
  float battVolt = (analogRead(battPin) * mult + bOffset);  // float vs. int in mV?
          //    Serial.println("main loop just read battV");
  static float baseVolt = battVolt;  // batt V in mV when we started discharging, set once
  static float battRange = (baseVolt - targVolt);  // range from starting high point to target V, set once
          //     Serial.println (battRange);          // should stay fixed
          //     Serial.print("done yet? "); Serial.println(done);
  while(done != 1)       //do this until done is true, target V reached, bypassed when done
   {             
    interval = (millis() - prevTime);   // ms. elapsed since loop start or last printVal job
                                        // will determine whether to print or wait
      //   Serial.print("interval now  "); Serial.println (interval);
    float battVolt = (analogRead(battPin) * mult + bOffset);  // refresh battVolt 'while looping'
    float deltaV  = (battVolt-targVolt);  //  how far batt is from target now
      //   Serial.print("delta  "); Serial.println(deltaV);         // should drop as batt dc's
      //   delay(5000);

    if ( (deltaV > battRange * 0.5) && (interval >= 15 * 60000)) 
      {printVals(battVolt);  //print every 15 min while far above target V
      }
    if ( (deltaV <= battRange * 0.5) && (deltaV > battRange * 0.25) && (interval >= 10 * 60000))
      {printVals(battVolt);    //print every 10 min in this range
      }
    if ( (deltaV <= battRange * 0.25) && (deltaV > battRange * 0.13) && (interval >= 5 * 60000))
      {printVals(battVolt);    //print every 5 min
      }
    if ( (deltaV <= battRange * 0.13) && (deltaV > battRange * 0.06) && (interval >= 3 * 60000))
      {printVals(battVolt);    //print every 3 min
      }
    if ( (deltaV <= battRange * 0.06) && (deltaV >1) && (interval >= 60000))
      {printVals(battVolt);    //print every 1 min
      }
    if (deltaV <= 1)     // target V reached
      {done = 1;        // stops while loop from repeating, exits to main loop
      break;
      }
     delay(2000);      // while, you wait
   }  // end while loop
               // need way to exit while loop if none of above are true? no, remove source or reset

      //   Serial.println("exit while loop");

  delay(2000);     // no reason to main loop any faster
  
  if (done == 1)  // main loop does this block repeatedly when done is true
   {  // print a fresh value
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
}  //end main loop

void printVals(int battV)    //prints current mV as integer, resets prevTime
{  
  int pinSees = (analogRead(battPin));
  Serial.print("elapsed min.= "); Serial.print((millis() - starTime) / 60000);
  Serial.print(" "); Serial.print("AnalRead= "); Serial.println(pinSees); 
  Serial.print("batt= "); Serial.print(battV);Serial.println(" mV");
  prevTime = millis();  //tried to print battVolt -- failed to update even tho a global
}  //end printVals

