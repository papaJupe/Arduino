/* USsr04 avg LCD -- Ultrasonic module to 1602 LCD display
  display averaged distance to onboard LCD shield
  needed significant editing to work, edit 1707 for SR-04 US module;
  Uses: dist_avg reads using exp moving average, pulseIn

   LCD RS pin to digital pin 8
   LCD Enable pin to digital pin 9
   LCD D4 pin to digital pin 4
   LCD D5 pin to digital pin 5
   LCD D6 pin to digital pin 6
   LCD D7 pin to digital pin 7
   LCD BL pin to digital pin 10
   KEY pin to analog pin 0
   Vcc pin to  +5         // above all hardwired w/ shield

   Trig pin to digital pin 3
   Echo pin to digital pin 12 -- pulseIn doc says it needs interrupt pin, but not true

*/

#include <LiquidCrystal.h>
#define CM 1      //Centimeter,  1 tells loop to display CM
   // Inch,  get displayed otherwise
#define TP 3     //Trigger pin
#define EP 12     //Echo pin

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // had 13 here, wrong

void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  lcd.setCursor(0, 0);
  Serial.begin(9600);       // 9600 bps, for Ser. Mon. if on
  pinMode(TP, OUTPUT);      // TP output pin for trigger
  pinMode(EP, INPUT);       // EP input pin for echo
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); // 5 v for US module power
}

void loop()
{ 
  static uint16_t distAvg = 69;
  delay(100);
  long microseconds = TP_init(); // activates the pulser, gets uS back
  unsigned int dist = Distance(microseconds, CM);
  // calc expon MA
  distAvg = round((distAvg * 4) + dist) / 5;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("dist : ");
  lcd.print(distAvg);
  if (CM) lcd.print(" cm");
     else lcd.print(" in");
  Serial.println(distAvg);

}  // end loop

long TP_init()
{
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  digitalWrite(TP, HIGH); // make Trig pin HIGH >= 10us
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  // pulse read waits for the echo pin to go HIGH, and
  // returns the length of the echo pulse in microseconds
  long microseconds = pulseIn(EP, HIGH);
  return microseconds;
}

uint16_t Distance(long time, int flag)  // returns unsigned int
{
  uint16_t distance;
  if (flag) // display in cm.
    distance = (time * 17) / 1000;
  // distance  = ((Duration of high level)*(Sonic :340m/s))/2
  // = ((Duration of high level)*(Sonic :0.034 cm/us))/2
  // = ((Duration of high level)/(Sonic :29.4 cm/us))/2
  else  // 1122 ft/sec   13464 in/sec   0.0135 in/uS / 2 for bounce
    distance = (time * 67) / 10000;       // INches
    // delay(10); 
  return distance;
}  // end Distance

