/****************************************************************************************
**  This is'custom' LINX firmware for use with the Arduino Uno
**  with the serial interface enabled. Upload from Ardu IDE instead
**  of LV Wizard/prebuilt, adding LCD plus I2C distance device
**
** mod by AM to use onboard 16x2 LCD display as well as LINX display
   works OK for analog-read, this code uses I2C device, display to LCD
****************************************************************************************/

//Include Peripheral Libraries for LINX, as needed for ___ device
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>
#include <VL6180X.h>  // laser distance sensor

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// #include <dummy.h>  // from libr/LINX, so Ardu IDE shows firmware libs
// for various boards

//Include Device Specific Header w/ Sketch>>Import Library (e.g. uno.h)
//Include some LINX Listener w/ Sketch>>Import Library (e.g. LinxSerialListener.h)
#include <LinxArduinoUno.h>
#include <LinxSerialListener.h>

//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoUno* LinxDevice;
VL6180X sensor;  // sensor instance

//Initialize LINX Device And Listener
void setup()
{
  // Instantiate The LINX Device
  LinxDevice = new LinxArduinoUno();
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  Wire.begin();
  // The LINX Listener Is Pre Instantiated, Call Start And Pass
  // A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);

  sensor.init();
  sensor.configureDefault();
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
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  delay(50);
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
  int als = sensor.readAmbientContinuous();

  // raw output seems 12 mm too high
  int rng = sensor.readRangeContinuousMillimeters() - 10;

  lcd.clear(); // .write does not work here for numbers
  // lcd.print("raw pin: "); lcd.print(analogRead(A0));
  lcd.print("ambient: ");
  lcd.print(als);
  lcd.setCursor(0, 1);
  lcd.print("range mm: ");
  lcd.print(rng);
  delay(10);
  
}  // end loop


