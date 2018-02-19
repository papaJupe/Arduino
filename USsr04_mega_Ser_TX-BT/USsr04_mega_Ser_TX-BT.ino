/* USsr04 avg Ser TX-BT -- US sr04 module on Mega w/ BT2 ser dongle, 
   send bytes over Ser_ to prog like Proc on PC, also ser. mon. sees
   Ardu I/O; BT mod adds sending pot data, uses BT serial dongle 
   on Ardu mega for wireless ser. to ser. mon. or Proc app

  Uses: gets dist_avg using exp moving average, pulseIn, Ser2 + BT2
  for comm w/ remote PC, TX=16, RX=17 on Mega

  Setup for MEGA: SR-04 US --
   Vcc pin to  +5 from pin 11    // w/ Proc
   Trig pin to digital pin 12
   Echo pin to digital pin 13 -- pulseIn doc wrong, !needs interrupt pin
   gnd to pin 13+1
   
  Ser_ comm to BT 2 module, ser2 pins: TX=16, RX=17;
   
  to send anal sensor data: pot voltage from 53, signal to A14
*/

#include <elapsedMillis.h>  // to .print at interval

#define CM 1      //Centimeter,  1 tells loop to display Cm, Inch if 0
#define TP 12     //Trigger pin
#define EP 13     //Echo pin

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 1000;  // ms
void setup()
{
  Serial.begin(115200);    // speed for Ser. Mon. if on
  Serial2.begin(115200);  // ser2 speed must = BT dongle UART speed
  pinMode(TP, OUTPUT);   // TP output pin for trigger
  pinMode(EP, INPUT);     // EP input pin for echo
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH); // 5 v for US module power

  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH); // 5v for pot
  pinMode(15, OUTPUT);  // 15 gnd is next to Ser2 tx
  digitalWrite(15, LOW); // grnd for level shift
}

void loop()
{
  static uint16_t distAvg = 690; // need to start somewhere, anywhere

  if (Serial2.available()) // Proc. wants more, send latest values
  { // one .read just clears one input char, while clears multiple
    while (Serial2.available()) {
      char ch = Serial2.read(); // data not used yet, just clears buffer
      Serial.print(ch);
    }
    Serial.println();
    // send distance (mm default) as bytes to Proc, prints space+one char in ser. mon.
    delay(2);
    Serial2.print("Analog input A_: ");
    Serial2.println(analogRead(A14));

    Serial2.print("US distance: ");
    //    Serial2.write(distAvg / 128); // hi byte
    //    Serial2.write(distAvg % 128); // lo byte
    Serial2.print(distAvg);  // sending int as chars
    Serial2.write(10);   // \n

    Serial.println(distAvg);  // ser. mon. shows value sent
  }   // end if ser2 available, incoming probe

  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // need to wait for value? v.i.--this is OK up to 300cm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;
  // print to ser. mon. at some sane interval
  //    if (timeElapsed > interval){
  //      { Serial.print("dist : ");
  //        Serial.print(distAvg);
  //      if (CM) Serial.println(" mm");
  //     else Serial.println(" in");
  //      //  lcd.setCursor(0, 1);
  //      //  lcd.print("uS: ");
  //      //  lcd.print(microseconds);
  //
  //     timeElapsed = 0;
  // }

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

uint16_t Distance(uint32_t time, int flag) // ret distance as unsigned int
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

