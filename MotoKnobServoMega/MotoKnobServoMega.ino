
/* MotoKnobServoMega -- Mead's mod to control a motor speed or servo using map function
  -- assuming 90 deg sent to servo obj = 1500 uS pulse width = motor stopped or mid pt;
  below that, motor runs back or CCW; above, it runs forward or CW; also works for positioning
  mod for mega uses D pins for ground/5v of pot, but powering servo fails from D pins
*/

#include <Servo.h>  // lib defaults 0 -> 544 uS, 180 -> 2400 uS; servo goes to whatever its min/max are

Servo tiltT;  // creates servo object to control something

#define potpinR A14  // analog pin to read the potentiometer voltage
float valaR;    //  variable reads the value from the analog pin R
int valR;     // mapped int to write a variable width pulse to output pin

void setup()
{
  pinMode(potpinR, INPUT);   // A14 input for R
  pinMode(53, OUTPUT); //  ground for pot
  digitalWrite(53, LOW);
  pinMode(51, OUTPUT); // 5v for pot
  digitalWrite(51, HIGH);

  Serial.begin(9600);   // for printout of values

  tiltT.attach(2);     // attach some dig (needs PWM? - no) pin to servo,
  //  or e.g. (pin,default min 544,max 2400)

}  //end setup

void loop()
{
  valaR = analogRead(potpinR);       // reads the volts from the potentiometer (0 to 1023)
  static float prevAvgR = valaR;  // just set once, first loop

  int newAvgR = round((valaR + 4 * prevAvgR) * 0.2); // ? faster to * than / by X
  valR = map(newAvgR, 40, 980, 2000, 1000); // scale it to degrees for servo (full range = 0 to 180 or partial range)
  // servo position center = 90, pulse = 1500 uS, motor still
  // 70-110 gives range of 1280 - 1680 uS

  tiltT.writeMicroseconds(valR);   // sets the servo position (or motor speed) according to the scaled value

  if (Serial.available())      //type any letter to get printout of vals, mostly for debugging
  { Serial.read();  // serves no purpose except to clear serial buffer
    Serial.print(newAvgR);  Serial.print("  "); Serial.println(valR);
    if (tiltT.attached()) Serial.println("on some pin");
    else Serial.println("nada");
  }   //end if

  // print out new values if different
  if (newAvgR != round(prevAvgR))  // only print if it's changed
  { //print to Ser Mon if it's on
    Serial.print("pinAvg= ");
    Serial.print(newAvgR);
    Serial.print(" angle= ");
    Serial.println(valR);
  }

  prevAvgR = newAvgR;

  delay(100);                // smaller delay = gets there faster
}  // end loop

