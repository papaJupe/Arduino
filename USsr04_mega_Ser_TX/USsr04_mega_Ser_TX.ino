/* USsr04 avg Ser TX -- US sr04 module on Mega w/ ESP shield, send bytes
    over Ser_ to prog like Proc on PC, also view Ardu's I/O w/ ser. mon.; test
    to later send data via wifi UDP to remote program like Proc SerialDataReadGraph

  Uses: gets dist_avg using exp moving average, pulseIn, udp .write bytes, Ser3
  for comm w/ remote PC, TX=14, RX=15 on Mega

  Setup for MEGA: serial1-3 pins connect via dongle to PC for comm w/ Proc
   Vcc pin to  +5 from pin 11    // via shield pass thru
   Trig pin to digital pin 12
   Echo pin to digital pin 13 -- pulseIn doc says it needs interrupt pin, but it doesn't
   gnd to pin 13+1
   Ser_ comm to remote PC, ser1 is TX=18, RX=19
*/

#include <elapsedMillis.h>  // to .print at interval

#define CM 1      //Centimeter,  1 tells loop to display Cm, Inch if 0
#define TP 12     //Trigger pin
#define EP 13     //Echo pin

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 1000;  // ms

void setup()
{
  Serial.begin(9600);    // speed for Ser. Mon. if on
  Serial1.begin(9600);  // speed must = Proc settings on PC, remote comm
  pinMode(TP, OUTPUT);   // TP output pin for trigger
  pinMode(EP, INPUT);     // EP input pin for echo
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH); // 5 v for US module power
}

void loop()
{
  static uint16_t distAvg = 690; // need to start somewhere, anywhere

  if (Serial1.available()) // Proc. wants more, send latest value: rewrite for UDP
  { // alone just clears one input char, while clears multiple
    while(Serial1.available()) Serial1.read();  // data not used yet, just clears buffer
    // send distance (mm default) as bytes to Proc, prints space+one char in ser. mon.
    delay(2);
    Serial1.write(distAvg / 128); // hi byte
    Serial1.write(distAvg % 128); // lo byte
    //Serial1.write(10);   // lr
    Serial.println(distAvg);  // ser. mon. can see what's sent
  }

  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // how long to wait for value? v.i.--this is OK up to 300cm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;
  // print to ser. mon. at some sane interval
//  if (timeElapsed > interval)
//    { Serial.print("dist : ");
//      Serial.print(distAvg);
//      if (CM) Serial.println(" mm");
//      else Serial.println(" in");
//      //  lcd.setCursor(0, 1);
//      //  lcd.print("uS: ");
//      //  lcd.print(microseconds);
//      // Serial.println(distAvg);  // for ser. mon.
//      timeElapsed = 0;
//    }

  delay(100);
}  // end loop

uint32_t TP_init()
{
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  digitalWrite(TP, HIGH); // make Trig pin HIGH >= 10us
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  // pulse read waits for the echo pin to go HIGH, and
  // returns the length of the echo pulse in microseconds
  uint32_t microseconds = pulseIn(EP, HIGH);
  return microseconds;
}

uint16_t Distance(uint32_t time, int flag)  // returns distance as unsigned int
{
  uint16_t distance;  // SR04 only works to ~3m, so max time uS < 18K
  if (flag) // display in cm./1000 or mm /100
    distance = (time * 17) / 100; // want mm to demo sending large #
  // distance  = ((Duration of high level)*(Sonic:340m/s))/2
  // = ((Duration of high level)*(Sonic :0.034 cm/uS))/2
  else  // 1122 ft/sec   13464 in/sec   0.0135 in/uS / 2 for bounce
    distance = (time * 67) / 10000;       // INches
  return distance;
}  // end Distance

