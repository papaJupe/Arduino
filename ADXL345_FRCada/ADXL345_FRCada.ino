/* ADXL345-FRCada adapted from Adafruit's _345 code for FRC accel breakout board
*  by AM 2016-05-02 -- v. https://github.com/adafruit/Adafruit_Sensor et. al.
* uses their Unified sensor libs, to standardize I/O for various sensors; nice goal,
* but it adds to memory use, and Ada does not appear to be keeping current or using
* much (just a few sensors coded for it). So ultimate usefulness is dubious.
*/

#include <Wire.h>
#include "Adafruit_Sensor.h"  // these are libs added to my lib folder, from github ...
#include "Adafruit_ADXL345_U.h"  //  edited for FRC device; add lots of progmem overhead

/* make I2C type object for this sensor, give it a sensorID # (not same as addr or devID) */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/* sketch has 3 functions, others in library */

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{  // rate the device is measuring internally, not same as our sampling rate
  Serial.print  ("Data Rate:  "); 
  
  switch(accel.getDataRate())
  {
     case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:+/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}  // end displayRange

void setup(void) 
{
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);  // voltage sources for device, so 4 leads exit together
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v
  
  delay(1000); // allow time for device to power up
  
//#ifndef ESP8266  this is some wireless device so should have nothing to do w/ Leo,Micro
//  while (!Serial); // for Leonardo/Micro/Zero
//#endif

  Serial.begin(9600);  // can use any value -- must match Ser. Mon. setting
  // Serial.flush();  // flush buffer, try to stop initial trash print
  delay(10);
  Serial.println("Accelerometer Test with Adafruit Unified code");

  /* .begin inits the sensor, checking devID held in device reg 0x00 against val in .cpp; 
  should fail if ID bad, != 0xE5, may not fail if mismatch ?
  */
  if(!accel.begin())   // a false (0) returned means failure to init
    { delay (10); // ? could be smaller
      /* problem detecting the ADXL345 by devID, blame the user */
      Serial.println("WTF dude! no ADXL345 detected! Check your wiring!");
      while(1);  // stops here, you need to fix things and reset
    }
  delay(10);
  /* Set the range to whatever is appropriate for your project */
   accel.setRange(ADXL345_RANGE_2_G); //  default
  // accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);  // no such fx here or .cpp
  // displaySetRange(ADXL345_RANGE_4_G);  // does he mean accel.setRange ???
  // displaySetRange(ADXL345_RANGE_2_G); // default
   delay(10);  //  allow time for set/get data; possibly could be shorter
   accel.setDataRate(ADXL345_DATARATE_100_HZ);
    delay(10);
  /* Display some basic information on this sensor */
  displaySensorDetails();
   delay(10);
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
   delay(10);
  displayRange();
   delay(10);
  Serial.println("");
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event;  // this is a struct defined in Adafr_Sensors.h to hold multip vars
  accel.getEvent(&event);  // ? fx writes vals back to the var, we print them below
 
  /* Display 3 axes' acceleration, reported in m/s^2 -- units calc by library */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println("m/s^2 ");
  delay(1000);
}  // end loop
