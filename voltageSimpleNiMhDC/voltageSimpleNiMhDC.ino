/*
 voltageSimplerNiMh to monitor batt discharge, using while loop;
 makes beep when target V reached using external discharge circuit
 */

#define battPin A4         // input pin for batt volt; need to say A4, no

const int targVolt= 920; // discharge target in mV, varies with batt type, depth you want
// this is target for NiCd, NiMh before putting to storage

int battVolt;            // in mV, calcul from analRead * mult
// long intervl;
long prevTime;

boolean done = false;        // not done yet
float mult = 4.920;      // multiplier empirically determined from measured V, may vary betw boards
boolean liteOn = false;  // flash LED to show program looping

void setup()   // option to calibrate voltage readout / target
{  
  Serial.begin(9600);  // current app needs Ser Mon open
  pinMode(13, OUTPUT); // LED here
  pinMode(battPin, INPUT);


  // mV at pin
  battVolt =  round(analogRead(battPin) * mult);  // take initial reading
  Serial.print("AR x mult="); 
  Serial.print(battVolt); 
  Serial.print(" ");  
  Serial.println("need calib.? - type y or n + enter");
  delay(5000);
  // while(!Serial.available());  // while no serial input, hold the action
  // will program hang @ while even if no Ser Mon open ?  Yes, so comment out prn, re-upload,
  // and all runs fine without input. Ser. Mon. open/close will reset, start sketch again, allow
  // to check calib, etc. No blame.
  if (Serial.available())
  { 
    delay(40);  //flaky reading of chars unless some delay
    if  (Serial.read() == 'y')
    {
      calibrate();
    }
    else      // any char but y ends the setup; need braces? no
    Serial.println("calib done or bypassed"); // is this enough? yes
  }  //end if serial.avail

  printVals(battVolt);  // this inits prevTime global as well

}    //end setup

void loop()   // read the batt volt, print out value, tone when target reached
{  
  //  intervl = (millis() - prevTime);   // ms. elapsed since last printVal job etc
  // will determine whether to do stuff or wait
  // read the analog input pin
  battVolt = round(analogRead(battPin) * mult);  // nearest int in mV

  Serial.print("main loop says V = "); 
  Serial.println(battVolt);

  while(done == false)       // do this until done is true,  when target V reached
  {                    // may not need this while loop but makes fast flash if sensing

    battVolt = round(analogRead(battPin) * mult);  // keep refreshed battVolt while looping here
    delay(120);  // unsteady value read and passed unless signif. delay here!

    if ( (battVolt > targVolt) )
    {
      if (millis() > (prevTime + 3000)) 
      {
        printVals(battVolt); // print every 3 sec while sensing
        // do not break, keep looping
        // Serial.println("while loop just read/print batt V w/ prVa");
      }

    }
    if (battVolt <= targVolt)     // target V reached
    {
      done = true;        // stops this while loop
      break;
    }
    // need way to exit while loop if none of above are true? no,
    // not if variables to evaluate & break are inside this loop

    if(liteOn) digitalWrite(13,HIGH);
    else digitalWrite(13,LOW);
    liteOn = !liteOn;   // invert each while loop for fast flash if sensing

    delay(300);      // while, you wait
  }  // end while loop  and fast flash

  //   Serial.println("exit while loop");


  if ((done == true) && (millis() > prevTime + 5000))  // main loop does this toning block 
    // every 5 sec when done is true
  {
    Serial.print(battVolt);  
    Serial.println("  -- discharge target reached");  
    Serial.println ("NOW - STOP ME WHILE YOU CAN!");
    for (byte i=0; i<=4; i++)  // play finish tone every 5 sec
    {
      tone(8, 2700);   // don't need pin mode config for toner
      delay(300);
      noTone(8);
      delay(200);
    } // end tone loop;
    prevTime = millis();   // reset for toning interval
  }  // end terminal if: print and tone

  if(liteOn) digitalWrite(13,HIGH);
  else digitalWrite(13,LOW);
  liteOn = !liteOn;   // invert value each while loop for slow flash in main loop

  delay(1500);     // no reason to loop any faster

}  //end main loop

void calibrate()   // could, but doesn't return value, just resets global mult
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
      mult = (mult + 0.003);
      battVolt = round(analogRead(battPin) * mult);
      Serial.print("mult is now "); 
      Serial.print(mult); 
      Serial.print(" - volts read "); 
      Serial.println(battVolt);
    }
    else if(ch =='d')
    {
      mult = (mult - 0.003);
      battVolt = round(analogRead(battPin) * mult);
      Serial.print("mult is now "); 
      Serial.print(mult); 
      Serial.print(" - volts read "); 
      Serial.println(battVolt);
    }  //end else
    delay(100);
  }  // end while, presumably when an x is typed

  Serial.println("calib. done, starting discharge program");
  //     nothing returns ; mult global has been reset by calib function
}   //end calib function

void printVals(int battV)    // prints current mV as integer, resets prevTime
{  
  Serial.print("batt= "); 
  Serial.print(battV); 
  Serial.println(" mV");
  prevTime = millis();  // tried print battVolt, failed to update even tho a global
}  //end printVals
