/*
  VL53L0X toner polo uses single shot mode to take range
  measurements with the VL53L0X I2C TOF rangefinder. Based on
  vl53l0x Single example using the VL53L0X API.
  polo lib works w/ adafrut sensor -- better than their lib.
  The range readings are in mm., valid <20 to ~1200 mm w/
  white wall, normal room lite

  Uses:  wire lib for I2C, elapsedMillis

*/

#include <Wire.h>
#include <VL53L0X.h>
#include <elapsedMillis.h>  // to .print at interval, break tone

VL53L0X sensor;

// Uncomment next line to use long range mode. This
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
unsigned int interval = 1400;  // (in ms) for sane ser mon print

#define litePin 3  // LED output

void setup()
{
  pinMode(litePin, OUTPUT);
  // pin for indicator LED gnd (2/3)
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  // pot to adjust sensitivity -- can't use A4,5 (used by I2C)
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  pinMode(A2, INPUT);

  Serial.begin(9600);
  Wire.begin();  // I2C use default SCL SDA ardu pins

  if (sensor.init())
  {
    Serial.println("TOF sensor present");
    sensor.setTimeout(500);
    Serial.println("Polo VL53L0X 'single' shot enabled");
  }
  // won't get past here if TOF sensor not found

  //#if defined LONG_RANGE
  //  // lower the return signal rate limit (default is 0.25 MCPS)
  //  sensor.setSignalRateLimit(0.1);
  //  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  //  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  //  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  //#endif
  //
  //#if defined HIGH_SPEED
  //  // reduce timing budget to 20 ms (default is about 33 ms)
  //  sensor.setMeasurementTimingBudget(20000);
  //#elif defined HIGH_ACCURACY
  //  // increase timing budget to 200 ms
  //  sensor.setMeasurementTimingBudget(200000);
  //#endif

  // Start continuous back-to-back mode (take readings as
  // fast as possible). To use continuous 'timed' mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  // sensor.startContinuous(50);

} // end setup


void loop()
{
  static int avgRng = 1200;
  int senseSett = analogRead(A2);

  //int range = sensor.readRangeContinuousMillimeters();

  int range = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred())
  {
    Serial.println(" TIMEOUT");
  }

  // avg and use only values in legitimate sensing range
  if (range > 20 && range < 1200)
  {
    // expon MA smoothes output; trade speed 4 stable readout
    avgRng = (((avgRng * 3) + range) / 4);

    // only activate lite & tone if current 'range' valid
    if (avgRng <= senseSett + 200 && timeElapsed >= 200 && timeElapsed < 1200)
    {
      digitalWrite(litePin, HIGH);
      tone(9, 2700);   // don't need pin config for toner
    }
    else {
      digitalWrite(litePin, LOW);
      noTone(9);
    }
    // print to ser. mon. at sane interval
    if (timeElapsed > interval)
    {
      Serial.print("Distance (mm): ");
      Serial.println(avgRng);
      Serial.println("sensor Setting = " + String(senseSett, DEC));
      timeElapsed = 0;
    }  // end if time elapsed
  } // end valid range actions

  // current sensed 'range' is invalid
  else {
    digitalWrite(litePin, LOW);
    noTone(9);

    if (timeElapsed > interval)
    {
      Serial.print("reading out of limit: ");
      Serial.println(range);
      Serial.println("sensor Setting = " + String(senseSett, DEC));
      timeElapsed = 0;
    }
  }  // end invalid range actions

  delay(100);

}  // end loop
