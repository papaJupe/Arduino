/* USsr04megaTestMod -- US sr04 module on miniMega for module testing

  Uses: gets dist_avg using exp moving average, pulseIn, power, gnd,
  signal from adjacent pins, returns distance in mm

  Setup for miniMEGA:
   Vcc pin to  +5 from pin n 
   Trig pin to digital pin n+2
   Echo pin to digital pin n+4 -- pulseIn doc says it needs interrupt pin, but it doesn't
   gnd to pin n+6

*/

#include <elapsedMillis.h>  // to .print at interval

#define CM 1      //Centimeter,  1 tells loop to display Cm, Inch if 0
#define TP 41     //Trigger pin
#define EP 43     //Echo pin

elapsedMillis timeElapsed; // declare global so not reset every loop vs ? use static
unsigned int interval = 1000;  // ms

void setup()
{
  Serial.begin(9600);    // speed for Ser. Mon.
  
  pinMode(TP, OUTPUT);   // TP output pin for trigger
  pinMode(EP, INPUT);     // EP input pin for echo
  
  pinMode(39, OUTPUT);
  digitalWrite(39, HIGH); // 5 v for US module power
  
  pinMode(45, OUTPUT);
  digitalWrite(45, LOW); // gnd for US module power
}

void loop()
{
  static uint16_t distAvg = 690; // need to start somewhere

  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // how long to wait for value? v.i.--this is OK up to 300cm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;

// print to ser. mon. at sane interval
  if (timeElapsed > interval)
    { Serial.print("dist : ");
      Serial.print(distAvg);
      if (CM) Serial.println(" mm");
      else Serial.println(" in");
      //  lcd.setCursor(0, 1);
      //  lcd.print("uS: ");
      //  lcd.print(microseconds);
      timeElapsed = 0;
    }

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
  if (flag) // display in cm/1000 or mm /100
    distance = (time * 17) / 100; // want mm to demo sending large #
  // distance  = ((Duration of high level)*(Sonic:340m/s))/2
  // = ((Duration of high level)*(Sonic :0.034 cm/uS))/2
  else  // 1122 ft/sec   13464 in/sec   0.0135 in/uS / 2 for bounce
    distance = (time * 67) / 10000;       // INches
  return distance;
}  // end Distance
