/* USsr04 avg Ser TX -- Ultrasonic distance --> LCD & send bytes over Serial
  display averaged distance to onboard LCD shield, send data via Serial to
  remote program like Proc SerialDataReadGraph

  Uses: gets dist_avg using exp moving average, pulseIn, LCD display,.write
  bytes

   LCD RS pin to digital pin 8
   LCD Enable pin to digital pin 9
   LCD D4 pin to digital pin 4
   LCD D5 pin to digital pin 5
   LCD D6 pin to digital pin 6
   LCD D7 pin to digital pin 7
   LCD BL pin to digital pin 10
   KEY pin to analog pin 0
   Vcc pin to  +5         // above hardwired w/ shield

   Trig pin to digital pin 11
   Echo pin to digital pin 12 -- pulseIn doc says it needs interrupt pin, but not true

*/

#include <LiquidCrystal.h>
#define CM 1    //Centimeter, 1 tells loop to display CM, Inch otherwise
#define TP 11   //Trigger pin
#define EP 12   //Echo pin

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // had 13 here, wrong

void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  lcd.setCursor(0, 0);
  Serial.begin(9600);    // 9600 bps, for Ser. Mon. if on, else Proc serial
  pinMode(TP, OUTPUT);   // TP output pin for trigger
  pinMode(EP, INPUT);     // EP input pin for echo
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); // 5 v for US module power
}

void loop()
{ 
  static uint16_t distAvg = 690; // need to start somewhere, anywhere
  if(Serial.available())  // Proc. wants more, send latest value
   {   // just clears one input char, I'd like all cleared
    Serial.read();  // var not used, just clears buffer
    // send distance (mm default) as bytes to Proc
    Serial.write(distAvg/256);  // hi byte
    Serial.write(distAvg%256);  // lo byte
    }
  uint32_t microseconds = TP_init(); // activates the pulser, gets uS back
  delay(20); // how long to wait for value? v.i.--this works up to 300cm, 3m
  uint16_t dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("dist : ");
  lcd.print(distAvg);
  if (CM) lcd.print(" mm");
     else lcd.print(" in");
  lcd.setCursor(0, 1);  // second line
  lcd.print("uS: ");
  lcd.print(microseconds);
  // Serial.println(distAvg);  // for ser. mon.
   delay(100);
}  // end loop

uint32_t TP_init()  //tried uint16 but doesn't work
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

