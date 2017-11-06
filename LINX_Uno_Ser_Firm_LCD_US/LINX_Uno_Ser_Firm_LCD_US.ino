/****************************************************************************************
**  -- non-working LINX firmware for use with the Arduino Uno
**  with the serial interface, LCD display output of US device
**
**  For more information see:      www.labviewmakerhub.com/linx
**  For support visit forums at www.labviewmakerhub.com/forums/linx
**
** mod by AM to use onboard 16x2 LCD display w/ LINX display w/ US device
  setting echoPin to D3 (interrupt1)or LINX pin, neither work
  seems like pulseIn interferes w/ LINX, can't communicate nor does new fx
*************************************************/

//Include Peripheral Libraries for LINX, as needed for ___ device
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>

#include <LiquidCrystal.h>  // LCD works fine
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define echoPin 3 // pulseIn ??? needs D2,3 for onboard read --> lcd display

// #include <dummy.h>  // from libr/LINX, so Ardu IDE shows firmware libs for various boards
//Include Device Specific Header w/ Sketch>>Import Library (e.g. uno.h)
//Include some LINX Listener w/ Sketch>>Import Library (e.g. LinxSerialListener.h)
#include <LinxArduinoUno.h>
#include <LinxSerialListener.h>

//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoUno* LinxDevice;

//Initialize LINX Device And Listener
void setup()
{
  // Instantiate The LINX Device
  LinxDevice = new LinxArduinoUno();
  //The LINX Listener Is Pre Instantiated, Call Start And Pass
  // A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);
  // different order of these didn't help
  lcd.begin(16, 2);    // 16 col, 2 row LCD
  pinMode(echoPin, INPUT);
//  pinMode(2, OUTPUT);
//  digitalWrite(2, HIGH);  // voltage for device (done in VI)

}  // end setup

void loop()
{
  // static uint16_t distAvg = 69; linx can't handle static
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  delay(50);
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
  // read echo pulse width and display locally -- makes Linx fail
  // uint16_t uS = pulseIn(echoPin, HIGH); linx cant handle pulseIn?
//  uint16_t uS = readPulse(echoPin);  or this fx to read pulse width 
//  uint16_t distance = uS * 0.017;
  //  distAvg = round((distAvg * 4) + distance)/5;

  lcd.clear(); // .write does not work here for numbers
  lcd.print("dist:"); lcd.print("d"); lcd.print(" cm");

} // end loop

unsigned long readPulse(int pin)
{
  unsigned long rise_time;  // time of the rising edge
  boolean last_state = LOW;   // previous pin state
  unsigned long pulse_len = 0;    // default return value
  unsigned long starTim;
  do {
    starTim = millis();  // timeout for this loop
    boolean state = digitalRead(pin);      // current pin state
    // On rising edge: record current time.
    if (last_state == LOW && state == HIGH) rise_time = micros();

    // On falling edge: report pulse length.
    if (last_state == HIGH && state == LOW)
    {
      unsigned long falling_time = micros();
      pulse_len = falling_time - rise_time;
    }
    last_state = state;
  }     // note final semicolon
  while (pulse_len == 0 && (millis() - starTim) < 200 );

  return pulse_len;
}   // end readPuls

