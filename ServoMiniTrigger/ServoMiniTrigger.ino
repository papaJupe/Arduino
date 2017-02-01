
/* ServoMiniTrigger -- mod to use external trigger on interrupt 1 for sending servo pulses,
 so I can display on PC's screen in sync
 
 built from MotoKnobServo, to use 2 channel servo box, trigger from AD pixelPulse on PC
 
 mod for Nano uses Anal pins for ground/5v of pot; my defective nanos can't handle servo and Serial
 in same sketch!
 */

#include <Servo.h>  // lib defaults 0 -> 544 uS, 180 -> 2400 uS; servo goes to whatever its min/max are

Servo mymoto;  // creates servo object to control something

int potpin = A2;  // analog pin to read the potentiometer voltage
float valL;    //  variable reads the value from the analog pin L pot, global


void setup()
{
  pinMode(potpin, INPUT);   // A4 input for R
  pinMode(14, OUTPUT); // A0 is ground for pot
  digitalWrite(14, LOW);
  pinMode(15, OUTPUT); // A1 outputs 5v for pot
  digitalWrite(15, HIGH);

  pinMode(3, INPUT);   // pin for interrupt #1, input signal from device here, no pullup/down

  // Serial.begin(9600);   // for printout of values, my nano/mini ch340 can't handle Serial & servo
  mymoto.attach(10);     // attach some dig (needs PWM? - no) pin to servo, or e.g. (pin,default min 544,max 2400)
  attachInterrupt(1, sendServo, RISING);

}

void loop()
{
  valL = analogRead(potpin);       // reads the volts from the potentiometer (0 to 1023)
  // static float prevAvgR = valaR;  // just set once, first loop

  // int newAvgR = round((valaR + 4 * prevAvgR) * 0.2); // ? faster to * than / by 9
  valL = map(valL, 5, 1010, 180, 0); // scale it to degrees for servo (full range = 0 to 180 or partial range)
  // servo position center = 90, pulse = 1500 uS, motor still
  // 70-110 gives range of 1280 - 1680 uS

  //  if (Serial.available())      //type any letter to get printout of vals, mostly for debugging
  //   { Serial.read();  // serves no purpose except to clear serial buffer
  //     Serial.print(newAvg); Serial.print("  "); Serial.println(val);
  //   }   //end if


  delay(100);                           // smaller delay = gets there faster
}  // end loop

void sendServo()  // ISR, triggered by input of pulse to D3; how fast can it respond ?
{
  // for (byte i = 0; i < 5; i++) // show 5 pulses, time for more ?, or let pulses repeat
    mymoto.write(valL);    // sets the servo position (or motor speed) according to the scaled value

}

