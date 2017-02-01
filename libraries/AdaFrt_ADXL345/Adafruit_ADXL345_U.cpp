/**************************************************************************/
/*!
    orig file name: Adafruit_ADXL345_U.cpp, full lib download from 
    https://github.com/adafruit/Adafruit_ADXL345/archive/master.zip
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)
    minor mods/comment by AM 160425

    The ADXL345 is a digital accelerometer with 13-bit resolution, capable
    of measuring up to +/-16g.  This driver communicates using I2C (looks like SPI too).

    Made originally for the Adafruit ADXL345 breakout
    ----> https://www.adafruit.com/products/1231

    @section  HISTORY
    
    v1.1 - Added Adafruit_Sensor library support
    v1.0 - First release
*/
/**************************************************************************/
#include "Arduino.h"

#include <Wire.h>

#include <limits.h>

#include "Adafruit_ADXL345_U.h"

/**************************************************************************/

/*!
    @brief  Instantiates a new ADXL345 obj using I2C if constr gets a double (setting 
    an arbitrary ID) from constr call; this != address or deviceID stored on device
*/
/**************************************************************************/
Adafruit_ADXL345_Unified::Adafruit_ADXL345_Unified(int32_t sensorID) 
{
  _sensorID = sensorID;
  _range = ADXL345_RANGE_4_G;  // reset to 2 G in setup()
  _i2c = true;    // tells rest of program how to read/write
}

/**************************************************************************/
/*!
    @brief  Instantiates a new ADXL345 class in SPI mode if constr call sends these args
*/
/**************************************************************************/
Adafruit_ADXL345_Unified::Adafruit_ADXL345_Unified(uint8_t clock, uint8_t miso, uint8_t mosi, uint8_t cs, int32_t sensorID) 
{
  _sensorID = sensorID;
  _range = ADXL345_RANGE_2_G;
  _cs = cs;
  _clk = clock;
  _do = mosi;
  _di = miso;
  _i2c = false;
}

/**************************************************************************/
/*!
    @brief  Setup the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
bool Adafruit_ADXL345_Unified::begin() 
{
  if (_i2c)
    Wire.begin();
  else {        // SPI config
    pinMode(_cs, OUTPUT);
    pinMode(_clk, OUTPUT);
    digitalWrite(_clk, HIGH);
    pinMode(_do, OUTPUT);
    pinMode(_di, INPUT);
  }
 delay(20);
  /* Check connection -- is ID same for SPI and I2C, Y */ 
 uint8_t deviceid = getDeviceID();
 delay(40);
 Serial.print("got dev ID = "); Serial.println((byte)deviceid, DEC);  
  if (deviceid != 0xE5)    // supposed to be E5, 229
	  {
		/* No ADXL345 detected ... return false */
		Serial.println("correct ID is 0xE5");
		return false;
	  }
  
  // Enable measurements
  writeRegister(ADXL345_REG_POWER_CTL, 0x08);  
    
  return true;
}  // end begin
/**************************************************************************/
/*! 
    @brief  Read the device ID (can be used to check connection)
*/
/**************************************************************************/
uint8_t Adafruit_ADXL345_Unified::getDeviceID(void) 
{
  // Check device ID register  0x00  -- same for both protocols ?
  return readRegister(ADXL345_REG_DEVID);
}
/**************************************************************************/
/*!
    @brief  Sets the g range for the accelerometer
*/
/**************************************************************************/
void Adafruit_ADXL345_Unified::setRange(range_t range)
{
  /* Read the data format register to preserve bits */
  uint8_t format = readRegister(ADXL345_REG_DATA_FORMAT);

  /* Update the range bits in the register */
  format &= ~0x03;  // invert 3=>mask to keep high bits but zero out low 2: 11111100
  format |= range;  // add 2 bits of range back to it
  
  /* Make sure that the FULL-RES bit is enabled for range scaling */
  format |= 0x08;  // B00001000
  
  /* Write the format back to the register */
  writeRegister(ADXL345_REG_DATA_FORMAT, format);
  
  /* Keep track of the current range (to avoid readbacks) */
  _range = range;
}   // end setRange

/**************************************************************************/
/*!
    @brief  read g range for the accelerometer
*/
/**************************************************************************/
range_t Adafruit_ADXL345_Unified::getRange(void)
{
  /* Read the data format register to preserve its 2 bits, cast them to a range constant */
  return (range_t)(readRegister(ADXL345_REG_DATA_FORMAT) & 0x03); // mask shows last 2 bits
}

/**************************************************************************/
/*!
    @brief  set and read the data rate for the ADXL345 (controls power consumption)
*/

void Adafruit_ADXL345_Unified::setDataRate(dataRate_t dataRate)
{
  /* Note: The LOW_POWER bits are currently ignored and we always keep
     the device in 'normal' mode */
  writeRegister(ADXL345_REG_BW_RATE, dataRate);
}

dataRate_t Adafruit_ADXL345_Unified::getDataRate(void)
{
  return (dataRate_t)(readRegister(ADXL345_REG_BW_RATE) & 0x0F); // cast last 4 bits to rate const
}
/**************************************************************************/
/*!
    @brief  Abstract away platform differences: new vs. very old Arduino wire library
*/
/**************************************************************************/
inline uint8_t Adafruit_ADXL345_Unified::i2cread(void) 
{
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

inline void Adafruit_ADXL345_Unified::i2cwrite(uint8_t x) 
{
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}

/**************************************************************************/
/*!
    @brief  Abstract away SPI receive & send differences
*/
/**************************************************************************/
static uint8_t spixfer(uint8_t clock, uint8_t miso, uint8_t mosi, uint8_t data) 
{
  uint8_t reply = 0;
  for (int i=7; i>=0; i--) 
  {
    reply <<= 1;  //  = reply shifted left 1 place, v. Cookbook p. 68
    digitalWrite(clock, LOW);
    digitalWrite(mosi, data & (1<<i)); // mask according to i, i.e. if i=7, mask is 10000000
    digitalWrite(clock, HIGH);
    if (digitalRead(miso)) 
      reply |= 1;   // = binary OR mask with 1, i.e. smallest bit always = 1
  }  // end if
  return reply;
}  // end fx

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void Adafruit_ADXL345_Unified::writeRegister(uint8_t reg, uint8_t value) 
{
  if (_i2c) 
  {
    Wire.beginTransmission(ADXL345_ADDRESS);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();
  } else {
    digitalWrite(_cs, LOW);
    spixfer(_clk, _di, _do, reg);
    spixfer(_clk, _di, _do, value);
    digitalWrite(_cs, HIGH);
  }  // end if/else
}  // end writeRegis

/**************************************************************************/
/*!
    @brief  Reads 8-bits from the specified register
*/
/**************************************************************************/
uint8_t Adafruit_ADXL345_Unified::readRegister(uint8_t reg) 
{
  if (_i2c) 
  {
    Wire.beginTransmission(ADXL345_ADDRESS);
    i2cwrite(reg);  // pointer to reg you want
    Wire.endTransmission();
    Wire.requestFrom(ADXL345_ADDRESS, 1);
    return (i2cread());
  } else {
    reg |= 0x80; // read byte w/ SPI
    digitalWrite(_cs, LOW);
    spixfer(_clk, _di, _do, reg);
    uint8_t reply = spixfer(_clk, _di, _do, 0xFF);
    digitalWrite(_cs, HIGH);
    return reply;
  }   // end if/else
} // end readRegi

/**************************************************************************/
/*!
    @brief  Reads 16-bits from the specified register
*/
/**************************************************************************/
int16_t Adafruit_ADXL345_Unified::read16(uint8_t reg) 
{
  if (_i2c)
  {
    Wire.beginTransmission(ADXL345_ADDRESS);
    i2cwrite(reg);
    Wire.endTransmission();
    Wire.requestFrom(ADXL345_ADDRESS, 2);  // get 2 bytes
    return (uint16_t)(i2cread() | (i2cread() << 8));  // smallest byte sent 1st, this sums them
  } else {
    reg |= 0x80 | 0x40; // read byte | multibyte, binary OR mask, Cookbook p. 68
    digitalWrite(_cs, LOW);
    spixfer(_clk, _di, _do, reg);
    uint16_t reply = spixfer(_clk, _di, _do, 0xFF)  | (spixfer(_clk, _di, _do, 0xFF) << 8);
    digitalWrite(_cs, HIGH);
    return reply;
  }   // end if/else
}  // end read16

/**************************************************************************/
/*! 
    @brief  Gets the most recent X axis value as 2 bytes
*/
/**************************************************************************/
int16_t Adafruit_ADXL345_Unified::getX(void) 
{
  return read16(ADXL345_REG_DATAX0);
}

/**************************************************************************/
/*! 
    @brief  Gets the most recent Y axis value
*/
/**************************************************************************/
int16_t Adafruit_ADXL345_Unified::getY(void) 
{
  return read16(ADXL345_REG_DATAY0);
}

/**************************************************************************/
/*! 
    @brief  Gets the most recent Z axis value
*/
/**************************************************************************/
int16_t Adafruit_ADXL345_Unified::getZ(void) 
{
  return read16(ADXL345_REG_DATAZ0);
}

/**************************************************************************/
/*! 
    @brief  Gets the most recent sensor event
*/
/**************************************************************************/
bool Adafruit_ADXL345_Unified::getEvent(sensors_event_t *event) 
{
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));
  // uses constants from 2 .h libs to return vals in SI units: meters/sec/sec
  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = 0;  // getX,Y,Z returns the raw int value
  event->acceleration.x = getX() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  event->acceleration.y = getY() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  event->acceleration.z = getZ() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  
  return true;
}

/**************************************************************************/
/*! 
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void Adafruit_ADXL345_Unified::getSensor(sensor_t *sensor) 
{
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "ADXL345", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version   = 1;  // must be same size as struct sensors_event_t (looks > 36 to me) -- 1 expected
  sensor->sensor_id   = _sensorID;
  sensor->type   = SENSOR_TYPE_ACCELEROMETER;  // this was wrong before I edited
  sensor->min_delay   = 0;
  sensor->max_value   = -156.9064F; /* -16g = 156.9064 m/s^2  min/max reversed?*/
  sensor->min_value   = 156.9064F;  /*  16g = 156.9064 m/s^2  */
  sensor->resolution  = 0.03923F;   /*  4mg = 0.0392266 m/s^2 */ 
}
