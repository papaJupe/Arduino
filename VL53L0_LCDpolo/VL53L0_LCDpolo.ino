/* 
  VL53L polo shows use of continuous mode to take
  range measurements with the VL53L0X. It is based on
  vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.
  works w/ adafrut sensor, better than their code/lib
  The range readings are in units of mm., good to ~ 1200 w/
  white wall, normal room lite

  
*/

#include <Wire.h>
#include <VL53L0X.h>
#include <elapsedMillis.h>  // to .print at interval
#include <LiquidCrystal.h>

VL53L0X sensor;

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 2000;  // ms for local ser mon print

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible). To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);

  Serial.println("Polo VL53L0X continuous test");

} // end setup


void loop()
{
  static int avgRng = 100;
  int range = sensor.readRangeContinuousMillimeters();

  // Serial.print(sensor.readRangeContinuousMillimeters());
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
