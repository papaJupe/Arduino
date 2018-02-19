/* 
  VL53L0X polo shows use of continuous mode to take range
  measurements with the VL53L0X I2C TOF rangefinder. Based on
  vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.
  polo lib works w/ adafrut sensor, better than their lib.
  The range readings are in mm., good to ~ 1200 mm w/
  white wall, normal room lite

  Uses: LCD 16x2 display, wire lib for I2C, elapsedMillis,
  

*/

#include <Wire.h>
#include <VL53L0X.h>
#include <elapsedMillis.h>  // to .print at interval
#include <LiquidCrystal.h>

VL53L0X sensor;
// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE
// my test --> worse range in normal room lite

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED  neither improved range or accuracy
//#define HIGH_ACCURACY

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Wire.begin();  // use default SCL SDA ardu pins

  sensor.init();
  sensor.setTimeout(500);
  
#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif

  // Start continuous back-to-back mode (take readings as
  // fast as possible). To use continuous 'timed' mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(50);

  Serial.println("Polo VL53L0X continuous test");

} // end setup


void loop()
{
  static int avgRng = 100;
  int range = sensor.readRangeContinuousMillimeters();
  // was Serial.print(sensor.readRangeContinuousMillimeters());
  
  //int range = sensor.readRangeSingleMillimeters();
  
  if (sensor.timeoutOccurred()) 
  {
    Serial.println(" TIMEOUT");
  }

  // expon MA smoothes output; choose speed vs stability
  avgRng = (((avgRng * 6) + range) / 7);

    lcd.clear();
    // lcd.setCursor(0, 0);
    lcd.print("range mm: ");
    lcd.print(avgRng);
  
  // print to ser. mon. at sane interval
  if (timeElapsed > interval)
  {
    Serial.print("Distance (mm): ");
    Serial.println(avgRng);    
    timeElapsed = 0;
  }  // end if time elapsed

  delay(100);
}  // end loop
