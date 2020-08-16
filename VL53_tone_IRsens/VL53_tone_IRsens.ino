/*

  VL53L0X toner + IR uses single shot mode to get range
  measurements with the VL53L0X I2C TOF rangefinder ||
  with IR dig sensor, whichever is present and powered on;
  both lite LED when triggered, TOF also sounds tone.
  switch 5v power wire to chosen sensor, sketch adapts.
  The TOF readings are in mm., valid <20 to ~1200 mm w/
  white wall, normal room lite. No sensitivity setting on
  device. IR range depends on device & setting (when possible).

  Uses:  wire lib for I2C TOF device, elapsedMillis, digitalRead/
  Write, analogRead, String() cast to print int

*/

#include <Wire.h>
#include <VL53L0X.h>
#include <elapsedMillis.h>  // to .print at interval, interupt tone

VL53L0X sensor;

elapsedMillis timeElapsed; // declare global so not reset every loop
unsigned int interval = 1400;  // (in ms) for toning & ser mon print
bool TOF = false; // whether VL53LOX device detected

#define irSignal 7     // digital pin input for IR only
#define vl53present 12  // input pulled high if VL53LOX is powered
#define litePin 3  // LED indicator lite for both sensor

void setup()
{
  pinMode(irSignal, INPUT_PULLUP);
  pinMode(vl53present, INPUT);
  pinMode(litePin, OUTPUT);

  // indicator LED (2/3) gnd
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

  if (digitalRead(vl53present))  // it's powered on
  { // no exception handling in IDE so must check if it's powered on
    if (sensor.init()) { // device detected
      Serial.println("TOF sensor present");
      TOF = true;
      sensor.setTimeout(500);
      Serial.println("Polo VL53L0X 'single' shot");
    } // end if init true
  }   // end if TOF power present

  else Serial.println("TOF sensor not detected");


  // Start continuous back-to-back mode (take readings as
  // fast as possible). To use continuous 'timed' mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)). not used in single mode.

} // end setup


void loop()
{
  // if IR sensor/switch pulls pin low, turn ON the LED
  // ignore if TOF was detected
  if (!TOF) digitalWrite(litePin, !digitalRead(irSignal));
  // digitalWrite(litePin, digitalRead(irSignal));  // lite on if dig input goes high

  static int avgRng = 1200;

  // read TOF sensor only if present
  if (TOF)
  {
    int senseSett = analogRead(A2); // adjust range sensitity

    //int range = sensor.readRangeContinuousMillimeters();

    int range = sensor.readRangeSingleMillimeters();

    if (sensor.timeoutOccurred())
    {
      Serial.println(" TIMEOUT");
    }

    // avg and use only values in legitimate sensing range
    if (range > 20 && range < 1200)
    {
      // expon MA smoothes output; trade speed 4 stable value
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
  } // end TOF sensor read/act

  delay(100);

}  // end loop
