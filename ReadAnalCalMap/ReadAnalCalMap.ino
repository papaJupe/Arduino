/*
  ReadAnalogCalMap --  (moving) averages analog pin input, 0-5 v. (as from batt
  discharger device with an op-amp reading 12 v batt) --  maps that to the 
  actual v. 11.6-13 being sampled; prints raw pin and calcul. voltage
  if it's changed; use to get params for map function in other sketches. simplest
  way to get map params for voltage only.
*/

#define PinIn A5

// for 8x2 LCD, include the library
#include <LiquidCrystal.h>

// initialize new lcd object with the Arduino pins
LiquidCrystal lcd(12, 11, 4, 5, 6, 7); // lib is smart enough to know if you're using 4 or 8 pins


int mapA = 172;      // default V map params, A for 11.6, B for 13.0 w/ USB+batt power to Ardu
int mapB = 963;  // so that Vcc is regulated 5 v -- must have stable reference for valid V

float newAvg;
float prevAvg;;

// setup runs once when you power on or Reset
void setup()
{

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(8, 2);

  // print to 2 lines of LCD, if it's present
  lcd.clear();  // first clear screen and set the cursor to (0,0)
  lcd.print("start");
  lcd.setCursor(0, 1);  // second line
  lcd.print("record");
}

void loop()
{
  // read the input on analog pin
  float sensorV = analogRead(PinIn);
  static float prevAvg = sensorV;  // just some starting place
  static int count = 0;
  float newAvg = (sensorV * 0.2 + prevAvg * 0.8); //update expon moving average of AR;
  // using float and mult improves accuracy of this calc.
  // convert analog reading (0 - 1023) to a voltage (11.5-13V) using map
  int mapAvg = round(newAvg);   // nearest int good for mapping, could use float?
  int voltage = map(mapAvg, mapA, mapB, 11600, 13000); // in mV, w/ volt divid, resolves ~25mV
  //int voltage = map(mapAvg, 759, 937, 3400, 4200); // in mV, resolves ~2 mV

  // print the new values to ser mon if changed or every 20 loops
  if ((mapAvg != round(prevAvg)) || count == 1)
  {
    Serial.print("pin out= "); Serial.print(mapAvg);
    Serial.print("  volt calc= "); Serial.println(voltage);

    // print raw pin read to LCD
    lcd.clear();  // first clear screen
    lcd.print("vPin " + String(mapAvg));
    lcd.setCursor(0, 1);  // second line
    lcd.print("mV " + String(voltage));
    prevAvg = newAvg;

  }
  count++;
  if (count >= 11) count = 0;
  delay (200);   // five loops / second
}
