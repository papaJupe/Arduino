
/* MotoKnobServoNanoD -- to control 2 motor speed or servo angle using map function
  -- assuming 90 deg sent to servo obj = 1500 uS pulse width = motor stopped or mid pt;
  below that, motor runs back or CCW; above, it runs forward or CW; also works for
  positioning servo arm. NB my defective nanos can't handle servo and Serial in same sketch!
  mod for 2 channel Nano control box uses Anal pins for ground/5v of pot;
  L channel normal bidir. action -- middle is off, 0 is full rev, 1024 is full fwd;
  This mod makes both pots send bidir. signal, rev - stop - fwd

  round box's CH341 Nano needed new driver for hiSi, Ardu Nano, 328P old bootloader; metro
  mini uses Uno, new SiLab driver for hiSi

*/

#include <Servo.h>  // lib defaults 0deg -> 544 uS, 180deg -> 2400 uS; 
//servo goes to whatever its min/max are
// PWM controlled motor uses 1000-2000 uS

Servo mymotoR;  // create servo objects
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
  pinMode(potpinR, INPUT);   // A6 input for R channel
  pinMode(18, OUTPUT); // A4 is ground for pot
  digitalWrite(18, LOW);
  pinMode(19, OUTPUT); // A5 gives 5v for pot
  digitalWrite(19, HIGH);

  //  // R pot position @ start sets mode: full CCW --> unidirectional, mid-range-->bidirect.
  //  maxR = (analogRead(potpinR) < 50) ? 1500 : 2400;

  pinMode(potpinL, INPUT);   // A2 input for L channel
  pinMode(A0, OUTPUT); // A0 is ground for pot
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT); // A1 gives 5v for pot
  digitalWrite(A1, HIGH);

  // Serial.begin(9600);   // for printout of values, my bad nano can't handle both Serial & servo
  // attach some dig pin to servo obj (needs PWM pin? - no) (pin,default min 544,max 2400)
  // attach(pin, min, max) - attaches to a pin, setting min and max values in useconds

  // both motors bidirectional full rev - 0 - full fwd
  mymotoR.attach(3, 1000, 2000);
  mymotoL.attach(10, 1000, 2000);
}

void loop()
{
  valaR = analogRead(potpinR);       // reads the volts from the potentiometer (0 to 1023)
  static float prevAvgR = valaR;  // just set once, first loop

  int newAvgR = round((valaR + 4 * prevAvgR) * 0.2); // avg 5 values, faster to * than /

  //
  valR = map(newAvgR, 5, 1020, 1000, 2000); // scale pot val to uS for moto (rev-0-fwd)
  // servo position center = 90, pulse = 1500 uS, motor still

  mymotoR.write(valR);       // sets the servo position (or motor speed) according to the scaled value

  prevAvgR = newAvgR;

  // L pot controls motor rev -0- fwd
  valaL = analogRead(potpinL);       // reads the volts from the potentiometer (0 to 1023)
  static float prevAvgL = valaL;     // just set once, first loop

  int newAvgL = round((valaL + 4 * prevAvgL) * 0.2); //  faster to * than /
  valL = map(newAvgL, 5, 1022, 1000, 2000); // scale it to uS for moto (rev-0-fwd)
  // pulse = 1500 uS, motor still

  mymotoL.write(valL);    // sets the servo position (or motor speed) according to the scaled value

  prevAvgL = newAvgL;
  delay(50);
}  // end loop

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
