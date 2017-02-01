
/* MotoKnobServoNanoB -- Mead's mod to control a motor speed or servo using map function
  -- assuming 90 deg sent to servo obj = 1500 uS pulse width = motor stopped or mid pt;
  below that, motor runs back or CCW; above, it runs forward or CW; also works for 
  positioning servo arm.  NB my defective nanos can't handle servo and Serial in same sketch! 
  mod for 2 channel Nano control box uses Anal pins for ground/5v of pot;  
  This modB has L channel normal pot action -- middle is off, 0 is full CCW, 1024 is full CW; 
  changed R channel to one direction, CCW, so 0 is 1500 ms, full on (1024) is 544 -- 
  should enable finer control of speed
*/

#include <Servo.h>  // lib defaults 0 -> 544 uS, 180 -> 2400 uS; servo goes to whatever its min/max are

Servo mymotoR;  // creates servo object to control something
Servo mymotoL;

#define potpinR  A6  // analog pin to read the potentiometer voltage
float valaR;    //  variable reads the value from the analog pin R
int valR;     // mapped int to write a variable width pulse to output pin

#define potpinL  A2  // analog pin to read the potentiometer voltage
float valaL;    //  variable reads the value from the analog pin 
int valL;     // mapped int to write a variable width pulse to output pin
// int prevAvg;  // use for moving avg, def in loop

void setup()
{
  pinMode(potpinR, INPUT);   // Ax input for R channel
  pinMode(18, OUTPUT); // A4 is ground for pot
  digitalWrite(18, LOW);
  pinMode(19, OUTPUT); // A5 gives 5v for pot
  digitalWrite(19, HIGH);

  pinMode(potpinL, INPUT);   // A2 input for L channel
  pinMode(A0, OUTPUT); // A0 is ground for pot
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT); // A1 gives 5v for pot
  digitalWrite(A1, HIGH);
  // Serial.begin(9600);   // for printout of values, NB bad nano can't handle both Serial & servo
     //   attach(pin, min, max) - Attaches to a pin, setting min and max values in microseconds
  
  mymotoR.attach(3, 540, 1500);   // attach some dig pin to servo (needs PWM pin? - no), 
     // or e.g. (pin,default min 544,max 2400)

  mymotoL.attach(10);
}

void loop()
{
  valaR = analogRead(potpinR);       // reads the volts from the potentiometer (0 to 1023)
  static float prevAvgR = valaR;  // just set once, first loop

  int newAvgR = round((valaR + 4 * prevAvgR) * 0.2); // avg 5 values, faster to * than / 
  valR = map(newAvgR, 2, 1020, 1500, 544); // scale pot val to uS for servo (off to full reverse)
  // servo position center = 90, pulse = 1500 uS, motor still
  // 70-110 gives range of 1280 - 1680 uS

  mymotoR.write(valR);       // sets the servo position (or motor speed) according to the scaled value
  //  if (Serial.available())     //type any letter to get printout of vals, mostly for debugging
  //   { Serial.read();  // serves no purpose except to clear serial buffer
  //     Serial.print(newAvg); Serial.print("  "); Serial.println(val);
  //   }   //end if
  // print out new values if different
  //  if (newAvg != round(prevAvg))  // only print if it's changed
  //  { //print to Ser Mon if it's on
  //    Serial.print("pinAvg= ");
  //    Serial.print(newAvg);
  //    Serial.print(" angle= ");
  //    Serial.println(val);
  //  }
  prevAvgR = newAvgR;
  
  valaL = analogRead(potpinL);       // reads the volts from the potentiometer (0 to 1023)
  static float prevAvgL = valaL;     // just set once, first loop

  int newAvgL = round((valaL + 4 * prevAvgL) * 0.2); //  faster to * than / 
  valL = map(newAvgL, 5, 1010, 180, 0); // scale it to degrees for servo (full range = 0 to 180 or partial range)
  // servo position center = 90, pulse = 1500 uS, motor still
  // 70-110 gives range of 1280 - 1680 uS

  mymotoL.write(valL);             // sets the servo position (or motor speed) according to the scaled value

  prevAvgL = newAvgL;
  delay(100);                           // smaller delay = gets there faster
}  // end loop

