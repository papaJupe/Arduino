/* adafrut lib and code perform worse than polo's: erratic readings, 
errors in debug printout:

sVL53L0X: PerformSingleRangingMeasurement
Range Status: 2 : Signal Fail
RANGE IGNORE THRESHOLD: 0.12
Measured distance: 858 <-- printout from debug code somewhere
Distance (mm): 1086  <-- these are moving-averaged printouts from sketch code,
so adjust slower than the debug printout

sVL53L0X: PerformSingleRangingMeasurement
Range Status: 2 : Signal Fail
RANGE IGNORE THRESHOLD: 0.24
Measured distance: 876
Distance (mm): 894

and large sketch size. 

*/

#include "Adafruit_VL53L0X.h"
#include <elapsedMillis.h>  // to .print at interval
#include <LiquidCrystal.h>

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print prn

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
} // end setup


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  static int avgRng = 100;

  // Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  int range = measure.RangeMilliMeter;

  // expon MA smoothes output numbers; choose speed vs stability
  // output erratic if fast
  avgRng = (((avgRng * 6) + range) / 7);

  if (measure.RangeStatus != 4)
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("range mm: ");
    lcd.print(avgRng);
  }
  // print to ser. mon. at sane interval
  if (timeElapsed > interval)
  {
 lox.rangingTest(&measure, true);
    if (measure.RangeStatus != 4)
    { // phase failures have incorrect data
      Serial.print("Distance (mm): ");
      Serial.println(avgRng);
    } else
    {
      Serial.println(" out of range ");
    }
    timeElapsed = 0;
  }  // end if time elapsed

  delay(100);
}  // end loop
