/* USsr04_MaxBot Avg_LCD -- Ultrasonic module to 1602 LCD display
  display averaged distance to onboard LCD shield to compare SR04
  to MaxBot analog sensor, on one display

  Uses: gets dist_avg using exp moving average, pulseIn

   anal pin in to A5
   Trig pin to digital pin 3
   Echo pin to digital pin 11 -- pulseIn doc says it needs interrupt pin, but not true

*/

#include <LiquidCrystal.h>
#define CM 0      //CM vs. IN:  0  for IN, 1 for CM
// Inch,  get displayed otherwise
#define TP 3     //Trigger pin
#define EP 11     //Echo pin
#define PinIn A5    // analog pin In

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

  int analDist;
  int sensorV;
  
void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  lcd.setCursor(0, 0);
  Serial.begin(9600);       // 9600 bps, for Ser. Mon. if on, else Proc ser. in ?
  pinMode(TP, OUTPUT);      // TP output pin for trigger
  pinMode(EP, INPUT);       // EP input pin for echo
  // pinMode(2, OUTPUT);
  // digitalWrite(2, HIGH); // 5 v for US module power
}

void loop()
{
  static uint16_t distAvg = 69; // for SR04 device
  static int prevAvg = analogRead(PinIn) + 5;  // for anal device

  long microseconds = TP_init(); // activates the pulser, gets uS back
  unsigned int dist = Distance(microseconds, CM);
  
  // calc expon MA of SR04 distance
  distAvg = round((distAvg * 0.8) + round(dist * 0.2);

  // read anal distance of Maxbotix
 
  sensorV = analogRead(PinIn);
  // update expon moving average of anal reads
  prevAvg = round((sensorV * 0.2) + (prevAvg * 0.8));
  //  use map from ~10mV (2 ticks) / inch, range ~ 6" to 360"
  analDist = map(prevAvg, 21, 204, 7, 80); // mapping ticks to inches for MaxBot

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SR04 : ");
  lcd.print(distAvg);
  if (CM) lcd.print(" cm");
  else lcd.print(" in");
  // Serial.println(distAvg);  // for ser. mon.
  // send distance (cm default) as bytes to Proc
  //  Serial.write(distAvg / 256); // hi byte
  //  Serial.write(distAvg % 256); // lo byte
  //  Serial.write('\n');  // Proc buffer expects \n to end message
  lcd.setCursor(0, 1);  // second line
  lcd.print("MxBot: ");
  lcd.print(analDist);
  lcd.print(" in.");

  delay(100);
}  // end loop

// 2 fx's for SR04 ranging
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

uint16_t Distance(long time, boolean flag)  // returns unsigned int
{ // as CM or IN depending on flag  1/0
  uint16_t distance;
  if (flag) // display in cm.
    distance = (time * 17) / 1000;
  // distance  = ((Duration of high level)*(Sonic :340m/s))/2
  // = ((Duration of high level)*(Sonic :0.034 cm/us))/2
  // = ((Duration of high level)/(Sonic :29.4 cm/us))/2
  else  // 1122 ft/sec   13464 in/sec   0.0135 in/uS / 2 for bounce
    distance = (time * 72) / 10000;   // INches (manual calib)
  // delay(10);
  return distance;
}  // end Distance
