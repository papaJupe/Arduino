/*
  Anal_Digi_VL53L0X_LCD shows use of continuous mode to take range
  measurements with the VL53L0X I2C TOF rangefinder. Based on
  vl53L0x_ContinuousRanging_Example.c from the VL53L0X API.
  polo lib works w/ adafrut sensor, better than ada's lib.
  The range readings are in mm., good to ~ 1200 mm w/
  white wall, normal room lite

  Now has input & display for 1 digital or 1 analog sensor w/
  power from digital pin 11, in addition to above range display

  Uses: LCD 16x2 display, wire lib for I2C, elapsedMillis,
 if dig signal pulls down the input, it turns ON the LED
*/

#include <Wire.h>. //i2c lib
#include <VL53L0X.h>
#include <elapsedMillis.h>  // to .print at interval
#include <LiquidCrystal.h>

VL53L0X sensor;

// #define LONG_RANGE
// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.
// my test --> worse range in normal room lite

// Uncomment ONE of next two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED  neither improved range or accuracy
//#define HIGH_ACCURACY

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // used for 16x2 shield
#define ApinIn A3     // analog 0 wired to switch array on 1602 board
#define DpinIn 2     // digital pin 2
#define LiteOut 13  // LED output for digital signal


void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Wire.begin();  // use default SCL SDA ardu pins

  sensor.init();
  sensor.setTimeout(500);

  pinMode(DpinIn, INPUT);
  digitalWrite(DpinIn, HIGH);  // internal pull-up, or use INPUT_PULLUP
  pinMode(LiteOut, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH); // 5v power for digi/anal device

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
  sensor.startContinuous();

  Serial.println("Polo VL53L0X continuous test");

} // end setup


void loop()
{ // start TOF sensor distance code
  static int avgRng = 200;
  int range = sensor.readRangeContinuousMillimeters();
  // was Serial.print(sensor.readRangeContinuousMillimeters());

  //int range = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred())
  {
    Serial.println(" TIMEOUT");
  }

  // expon MA smoothes output; choose speed vs stability
  avgRng = (((avgRng * 4) + range) * 0.2); 

  lcd.clear();
  // lcd.setCursor(0, 0);
  lcd.print("range mm: ");
  if (avgRng >= 1201) lcd.print(">1200");
  else lcd.print(avgRng);

  // print to ser. mon. at sane interval
  if (timeElapsed > interval)
  {
    Serial.print("Distance (mm): ");
    Serial.println(avgRng);
    timeElapsed = 0;
  }  // end if time elapsed

  // end VL53L0 code

  //digital sensor code

  // if dig signal pulls down the input, it turns ON the LED
  digitalWrite(LiteOut, !digitalRead(DpinIn));

  // analog sensor code
  int voltage;
  static int prevAvg = analogRead(ApinIn);  // set value for first loop only

  // read input on analog pin
  int sensorV = analogRead(ApinIn);
  int newAvg = round((sensorV + 4 * prevAvg) * 0.2); // update expon moving average of AR
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5000 mV):
  voltage = (int)(newAvg * 4.90);  // in mV, resolves +/- 2 mV; cast to int (mV)
  // set multiplier using voltmeter; varies some from pin to pin, board to board

  // serial print the new MA if different
  if (newAvg != prevAvg)
  { //print to Ser Mon if it's on
    Serial.print("anal pin = ");
    Serial.print(newAvg);
    Serial.print(" calc mV= ");
    Serial.println(voltage);
  } // end if changed value

  // print to 2nd line of LCD, if it's present
  lcd.setCursor(0, 1); // start on second line
  lcd.print("AR=");
  lcd.print(newAvg);  // the averaged AR
  lcd.print(" mV= ");
  lcd.print(voltage);
  prevAvg = newAvg;
  // end analog sensor code

  delay(50);
}  // end loop
