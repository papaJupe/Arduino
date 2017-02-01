/*! 
    orig file name: Adafruit_ADXL345_U.h, full lib download from 
    https://github.com/adafruit/Adafruit_ADXL345/archive/master.zip
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)
    major mods/comment by AM 160425 to use FRC breakout board

    This is a library for the Adafruit ADS1015 breakout board
      ----> https://www.adafruit.com/products/1231
     
    @section  HISTORY
    v1.0  - First release
*/

#include "Arduino.h"

#include <Adafruit_Sensor.h>  // separate lib needed from adafruit? Y

#include <Wire.h>

/*=========================================================
    I2C ADDRESS of device  -- why not a var in constr? shouldn't it be set(able) in sketch ?
    -----------------------------------------------------------------------*/
#define ADXL345_ADDRESS  (0x1D)    // 7 bit addr (29) for FRC bd;  use 0x53  (83) for Ada
// FRC seems to have ALT address pin hi;  Ada_'s low; D0 may be alt addr pin on FRC bd 
/*=========================================================================*/

#define ADXL345_MG2G_MULTIPLIER (0.00392)  // 4mg per lsb
/*======================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    #define ADXL345_REG_DEVID               (0x00)    // Device ID
    #define ADXL345_REG_THRESH_TAP      (0x1D)    // Tap threshold
    #define ADXL345_REG_OFSX                (0x1E)    // X-axis offset
    #define ADXL345_REG_OFSY                (0x1F)    // Y-axis offset
    #define ADXL345_REG_OFSZ                (0x20)    // Z-axis offset
    #define ADXL345_REG_DUR                 (0x21)    // Tap duration
    #define ADXL345_REG_LATENT            (0x22)    // Tap latency
    #define ADXL345_REG_WINDOW           (0x23)    // Tap window
    #define ADXL345_REG_THRESH_ACT     (0x24)    // Activity threshold
    #define ADXL345_REG_THRESH_INACT   (0x25)    // Inactivity threshold
    #define ADXL345_REG_TIME_INACT        (0x26)    // Inactivity time
    #define ADXL345_REG_ACT_INACT_CTL   (0x27)    // Axis enable control for activity and inactivity detection
    #define ADXL345_REG_THRESH_FF        (0x28)    // Free-fall threshold
    #define ADXL345_REG_TIME_FF             (0x29)    // Free-fall time
    #define ADXL345_REG_TAP_AXES            (0x2A)    // Axis control for single/double tap
    #define ADXL345_REG_ACT_TAP_STATUS  (0x2B)    // Source for single/double tap
    #define ADXL345_REG_BW_RATE             (0x2C)    // Data rate and power mode control
    #define ADXL345_REG_POWER_CTL           (0x2D)    // Power-saving features control
    #define ADXL345_REG_INT_ENABLE          (0x2E)    // Interrupt enable control
    #define ADXL345_REG_INT_MAP             (0x2F)    // Interrupt mapping control
    #define ADXL345_REG_INT_SOURCE        (0x30)    // Source of interrupts
    #define ADXL345_REG_DATA_FORMAT     (0x31)    // Data format control, includes range
    #define ADXL345_REG_DATAX0              (0x32)    // X-axis data 0
    #define ADXL345_REG_DATAX1              (0x33)    // X-axis data 1
    #define ADXL345_REG_DATAY0              (0x34)    // Y-axis data 0
    #define ADXL345_REG_DATAY1              (0x35)    // Y-axis data 1
    #define ADXL345_REG_DATAZ0              (0x36)    // Z-axis data 0
    #define ADXL345_REG_DATAZ1              (0x37)    // Z-axis data 1
    #define ADXL345_REG_FIFO_CTL            (0x38)    // FIFO control
    #define ADXL345_REG_FIFO_STATUS       (0x39)    // FIFO status
/*=========================================================================*/

/* Used in register 0x2C (ADXL345_REG_BW_RATE) to set bandwidth */
typedef enum
{   // these were all 0bxxxx, I changed to Bxxxx

  ADXL345_DATARATE_3200_HZ    = B1111, // 1600Hz Bandwidth   140µA IDD
  ADXL345_DATARATE_1600_HZ    = B1110, //  800Hz Bandwidth    90µA IDD
  ADXL345_DATARATE_800_HZ     = B1101, //  400Hz Bandwidth   140µA IDD
  ADXL345_DATARATE_400_HZ     = B1100, //  200Hz Bandwidth   140µA IDD
  ADXL345_DATARATE_200_HZ     = B1011, //  100Hz Bandwidth   140µA IDD
  ADXL345_DATARATE_100_HZ     = B1010, //   50Hz Bandwidth   140µA IDD (default value)
  ADXL345_DATARATE_50_HZ       = B1001, //   25Hz Bandwidth    90µA IDD
  ADXL345_DATARATE_25_HZ       = B1000, // 12.5Hz Bandwidth    60µA IDD
  ADXL345_DATARATE_12_5_HZ    = B0111, // 6.25Hz Bandwidth    50µA IDD
  ADXL345_DATARATE_6_25HZ     = B0110, // 3.13Hz Bandwidth    45µA IDD
  ADXL345_DATARATE_3_13_HZ    = B0101, // 1.56Hz Bandwidth    40µA IDD
  ADXL345_DATARATE_1_56_HZ    = B0100, // 0.78Hz Bandwidth    34µA IDD
  ADXL345_DATARATE_0_78_HZ    = B0011, // 0.39Hz Bandwidth    23µA IDD
  ADXL345_DATARATE_0_39_HZ    = B0010, // 0.20Hz Bandwidth    23µA IDD
  ADXL345_DATARATE_0_20_HZ    = B0001, // 0.10Hz Bandwidth    23µA IDD
  ADXL345_DATARATE_0_10_HZ    = B0000  // 0.05Hz Bandwidth    23µA IDD 

} dataRate_t;

/* Used in register 0x31 (ADXL345_REG_DATA_FORMAT) to set g range */
typedef enum
{
  ADXL345_RANGE_2_G           = B00,    // +/- 2g (default value)
  ADXL345_RANGE_4_G           = B01,   // +/- 4g
  ADXL345_RANGE_8_G           = B10,   // +/- 8g
  ADXL345_RANGE_16_G         = B11   // +/- 16g
} range_t;

/**************************************************************************/

class Adafruit_ADXL345_Unified : public Adafruit_Sensor   // a extends (abstr?) b
{
 public:
  Adafruit_ADXL345_Unified(int32_t sensorID = -1);  // proto for I2C
                              // proto for SPI
  Adafruit_ADXL345_Unified(uint8_t clock, uint8_t miso, uint8_t mosi, uint8_t cs, int32_t sensorID = -1);

  bool          begin(void);
  void          setRange(range_t range);
  range_t     getRange(void);  // returns one of the enum constants ?
  void           setDataRate(dataRate_t dataRate);
  dataRate_t   getDataRate(void);
  bool            getEvent(sensors_event_t*);
  void            getSensor(sensor_t*);
 
  uint8_t    getDeviceID(void);
  void        writeRegister(uint8_t reg, uint8_t value);
  uint8_t    readRegister(uint8_t reg);
  int16_t    read16(uint8_t reg);

  int16_t    getX(void), getY(void), getZ(void);
  
 private:  
  inline uint8_t  i2cread(void);
  inline void      i2cwrite(uint8_t x);

  int32_t _sensorID;  // this is arbitrary # sent from sketch to constr; not same as address or deviceID in reg 0x00
  range_t _range;   // an enum
  uint8_t _clk, _do, _di, _cs;
  bool    _i2c;
};  // end AF Sensor

