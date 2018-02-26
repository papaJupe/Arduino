/*
  VL6180 laser TOF range 2-18cm, 20-180mm & AmbientLightSense
  -- more on pololu.com/product/2489
  
  This example demonstrates how to use interleaved mode to
  take continuous range and ambient light measurements. The
  datasheet recommends using interleaved mode instead of
  running "range and ALS continuous modes simultaneously (i.e.
  asynchronously)".

  In order to attain a faster update rate (10 Hz), the max
  convergence time for ranging and integration time for
  ambient light measurement are reduced from the normally
  recommended defaults. See section 2.4.4 ("Continuous mode
  limits") and Table 6 ("Interleaved mode limits (10 Hz
  operation)") in the VL6180X datasheet for more details.

  Raw ambient light readings can be converted to units of lux
  using the equation in datasheet section 2.13.4 ("ALS count
  to lux conversion").

  Example: A VL6180X gives an ambient light reading of 613
  with the default gain of 1 and an integration period of
  50 ms as configured in this sketch (reduced from 100 ms as
  set by configureDefault()). With the factory calibrated
  resolution of 0.32 lux/count, the light level is therefore
  (0.32 * 613 * 100) / (1 * 50) or 392 lux.

  The range readings are in units of mm.
  Wiring: 3-5v - VIN, gnd, SDA - A4/AREF+1, SCL - A5/AREF+2
  mod 1709 AM - display to 16x2 LCD, MA smoothing, interval print
*/

#include <Wire.h>   // ardu lib for I2C devices
#include <VL6180X.h>
#include <LiquidCrystal.h>
#include <elapsedMillis.h>  // print @ some interval

VL6180X sensor;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
elapsedMillis timeElapsed;
uint16_t interval = 3000;  // print interval

void setup()
{
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Serial.begin(9600);  // for Ser.Mon. display only
  Wire.begin();

  sensor.init();
  sensor.configureDefault();

  // Reduce range max convergence time and ALS integration
  // time to 30 ms and 50 ms, respectively, to allow 10 Hz
  // operation (as suggested by Table 6 ("Interleaved mode
  // limits (10 Hz operation)") in the datasheet).
  sensor.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  sensor.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);

  sensor.setTimeout(500);

  // stop continuous mode if already active
  sensor.stopContinuous();
  // in case stopContinuous() triggered a single-shot
  // measurement, wait for it to complete
  delay(300);
  // start interleaved continuous mode with period of 100 ms
  sensor.startInterleavedContinuous(100);

}  // end setup

void loop()
{
  static int alsAvg = 0;  // for expon MA calc of als, rng
  static int rngAvg = 100;

  int als = sensor.readAmbientContinuous();

  // raw output needs tuning for conditions
  int rng = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred())
    Serial.println(" als TIMEOUT");
  if (sensor.timeoutOccurred())
    Serial.println(" rng TIMEOUT");

  // expon MA smoothes output numbers
  alsAvg = (((alsAvg * 4) + als) / 5);
  rngAvg = (((rngAvg * 4) + rng) / 5);

  if (timeElapsed > interval)
  {
    Serial.print("Ambient: ");
    Serial.print(alsAvg);
    Serial.print("\tRange: ");
    Serial.println(rngAvg);
    Serial.println();
    timeElapsed = 0;
  }

  lcd.clear();
  lcd.print("ambient: ");
  lcd.print(alsAvg);
  lcd.setCursor(0, 1);
  lcd.print("range mm: ");
  lcd.print(rngAvg);
  delay(10);

}  // end loop
