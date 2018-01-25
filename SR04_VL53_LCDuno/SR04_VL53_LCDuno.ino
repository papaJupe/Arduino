/*
  SR04+VL53 LCD uno uses polo's code vl53l0x_ContinuousRanging TOF
  (on adafrut board) + SR04 US module for demo of 2 rangefinders 
  on 1 LCD display
  Uses: polo vl53lox lib, elapsedMillis, liquidCrystal, wire lib,
  expon MA
  
  both range readings are in mm., TOF good to ~ 1200 w/
  white wall, normal room lite; US good to 3000 mm ?
  
   Trig pin to digital pin 3
   Echo pin to digital pin 11

*/

#include <Wire.h>
#include <VL53L0X.h>
#include <elapsedMillis.h>  // to .print at interval
#include <LiquidCrystal.h>
#define CM 1    //Centimeter, 1 tells loop to display CM, Inch otherwise
#define TP 3   //Trigger pin
#define EP 11   //Echo pin

VL53L0X sensor;

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Wire.begin();
  pinMode(TP, OUTPUT);   // TP output pin for US trigger
  pinMode(EP, INPUT);     // EP input pin for US echo
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); // 5 v for US module power

  sensor.init();
  sensor.setTimeout(500);

  // for continuous back-to-back mode (take readings as
  // fast as possible)(empty). I use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);

  Serial.println(" range find w/ VL53L0X continuous + SR04");

} // end setup


void loop()
{
  static uint16_t distAvg = 690; // US range, need to start somewhere
  static int avgRng = 200;
  int range = sensor.readRangeContinuousMillimeters();

  // Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred())
  {
    Serial.println(" TIMEOUT");
  }

  // expon MA smoothes output; choose speed vs stability
  avgRng = (((avgRng * 4) + range) / 5);

  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // how long to wait for echo # -- OK up to 3000mm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;

  lcd.clear();
  // lcd.setCursor(0, 0);
  lcd.print("TOF rng mm: ");
  lcd.print(avgRng);
  lcd.setCursor(0, 1);
  lcd.print("US rng: ");
  lcd.print(distAvg);
  if (CM) lcd.print(" mm");
  else lcd.print(" in");

  // print to ser. mon. at sane interval
  if (timeElapsed > interval)
  {
    Serial.print("TOF Distance (mm): ");
    Serial.println(avgRng);
    Serial.print("US Distance (mm): ");
    Serial.print(distAvg);
    timeElapsed = 0;
    Serial.println();  // blank line between data sets
  }  // end if time elapsed

  delay(80);
}  // end loop

uint32_t TP_init()  // triggers pulse, reads echo width
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
