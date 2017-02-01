//  FRC anal. Gyro & _345 Accel devices code combined; first calibrate in their own sketch
// displays Temp and Angle on LCD and Ser. Mon., accel on Ser. Mon.
//  (I2Cdev) and ADXL345 accelerometer class libs
//  10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//   2011-10-07 - initial release
//   2016-05-02 edited for FRC _345 board and gyro by AM

#include "Wire.h"

// I2Cdev and ADXL345 must be installed as libraries
#include "I2Cdev.h"
#include "ADXL345.h"

/*************** code for Gyro & Temp ********************/

#define gyroPin A0          // gyro output to analog pin
#define tempPin A1         // temp may be relevant for drift calculation

//  include the library and init for 8x2 LCD
#include <LiquidCrystal.h>
// initialize new object with the Arduino pins it uses
LiquidCrystal lcd(12,11,4,5,6,7);

const float gyroVolt = 4.78;  //Gyro nominally 5V; put in measured val w/ AC or USB power and LCD on board
float gyroZeroVolt = 2.33;   //Gyro nominally outputs input/2 @ rest, but needs re-init. in setup and later
const float gyroSens = .0065;  //test ADW gyro nominally puts out 7mV/deg/sec; but seems less sensitive
const float rotatThres = 0.015;   // minimum deg/.01 sec to recalc. angle - helps with gyro drifting
// but if too high, you see no response to slow rotation => integrated angle too small

// global vars used in loop et.al. to measure things

int tempV;  // raw anal read (AR) of tempPin 0-1020
float newTavgV;  // used in MA calc for smoothing
float prevTavgV;  // same

int gyroV;     // raw AR of gyroPin
float newGavgV;  // used in MA calc for smoothing; floats needed for reliable MA calc
float prevGavgV; // same, baseline ~ 495

float gyroRate;  // variously used in calculating angle
float currentAngle;  // holds current angle

float Tvolt;  // volts @ T pin, calc. from MA of T pin AR
float Toffset; // offset from volts @ nominal 25 deg
float calcTdegC; // calculated Temp

/***************  begin var defin for accel ********************/

// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 ((83) 7 bit default for SparkFun & Ada_ board)
// ALT high = 0x1D ((29) seems to be default for FRC board; ALT pin? D0 is 3.3 v (high)
// addr 0x3A in AD's sheet is 8 bit version, not used in software
ADXL345 accel = ADXL345(0x1D); // (29)

int16_t ax, ay, az;  // 16 bit, 2 byte, i.e. Ardu's basic int type

#define LED_PIN 13 // (Arduino Uno/Nano/Mini is 13)
bool blinkState = false;


void setup() 
{
  pinMode(A2,OUTPUT); // for convenience I use 2 anal pins as voltage sources for device
  pinMode(A3,OUTPUT); 
  digitalWrite(A2,LOW);  // grnd
  digitalWrite(A3,HIGH);  // '5' v

  // set up the LCD's number of columns and rows: 
  lcd.begin(8,2);

  // join I2C bus (I2Cdev library doesn't do this automatically; optional arg = this dev addr, if slave)
  Wire.begin();

  // initialize serial communication
  // (38400 works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(9600);
  Serial.flush();  // clear garbage from buffer, doesn't seem to work
  // initialize device -- fx in .cpp, uses param sent by ADXL345 constructor
  Serial.println("Initializing I2C devices...");
  accel.initialize();

  // verify connection -- doesn't really; calls getDeviceID() which is 'true' even if value returned
  // matches 0 (which I tried in .cpp and is not a real devID); doesn't test -ACK bit; real devID is 0xE5
  Serial.println("Testing device connections...");
  Serial.println(accel.testConnection() ? "ADXL345 connection made" : "ADXL345 connection failed");

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  delay(5);
  accel.setRate(ADXL345_RATE_100);  // var value of 10 = 100 hz meas rate internally
  delay(5);
  byte rate = accel.getRate();  // default of 100 hz returns value of 10 here; 9 means 50 hz etc.
  Serial.println("meas. rate is  " + String(rate) + "\n -- v. adxl345.h for equiv. hz");

  /***************** end accel setup, begin gyro setup  ************/
  
  prevTavgV = analogRead(tempPin);  // initial value for MA calc in loop
  prevGavgV = analogRead(gyroPin);  // same

  // get resting Zero voltage on G pin, initialize currentAngle to 0
  float delta;
  do     // do this until G pin stable, AR average not changing
  {
    gyroV = analogRead(gyroPin);
    newGavgV = (float)gyroV*0.25 + 0.75*prevGavgV; // updates expon moving avg of gyro raw AR
                                          
    lcd.clear();  // clear screen and set cursor to (0,0)       
    lcd.print("zeroing");
    lcd.setCursor(0,1);   // second line
    lcd.print("Gpin "); 
    lcd.print(newGavgV,1);  // one figure precis. for value ~ 493
    delay (500);
    delta = fabs(newGavgV - prevGavgV);  // float abs value
    prevGavgV = newGavgV;   
  } while(delta >= 1.0);  // we want average AR's to be stable, delta < 1 tick, to continue 
  
  gyroZeroVolt = (newGavgV / 1020) * gyroVolt;  // resting zero voltage, float 2.33-2.44
  // = value @ rest, changes after some movement, then drifts slightly
  // so periodic rezeroing of angle and recal. of this rest value is needed.

  currentAngle = 0;  // G pin should be stable and correspond to rest i.e. 0 angle
  lcd.clear();
  lcd.print("move-on");
  delay(1000);
}

void loop() 
{
  static unsigned int counter = 0; // how many loops; static sets it once on first loop

  tempV = analogRead(tempPin);  // all these AR values were ints but float needed for accuracy
  newTavgV = (float)tempV*0.25 + 0.75*prevTavgV;  // updates expon moving average of Temp pin AR

  gyroV = analogRead(gyroPin);
  newGavgV = (float)gyroV*0.25 + 0.75*prevGavgV;  //updates expon moving average of G pin AR
                            // floats needed to avoid accumulating errors in calc.  but
                            // division very time consuming, ? use mult. instead
  // calib code not here -- do in Gyro sketch first

  prevTavgV = newTavgV;
  prevGavgV = newGavgV;   // updates for MA calc

  // reset gyro vals depending on serial mon input; could call these from switch also
  if (Serial.available() > 0)  // type 0 into Ser. Mon. to recalib. resting gyroZeroVolt
          // & reset currAngle to 0; typing any other char just resets zeroVolt, not angle
  {
    char ch = Serial.read();
    if (ch == '0') reZero();  // reset zeroVoltage and angle
    else reBase();  // non-0 char, just reset the baseline G voltage to resting val.
  }


  /* to calib T: get present AR on T pin and measure T from ext thermom.; assume 9mV change/deg C; 
   calc what V. 25 C. would be (25.0-meas T)*0.009; add to meas V; put that # in offset formula
  below; Toffset = present reading's diff. from 25 deg. divide it by .009 & add it to 25 for result
  -- may be simpler way to calibrate T
   */
  Tvolt = (newTavgV / 1020) * gyroVolt;  // calc T voltage (float) from T pin AR; gVolt=Vcc
  // offset from voltage @ nominal 25 deg
  Toffset = Tvolt - 2.366; // 2.472 w/ 4.99 Vcc; was 2.361-6 w/ 4.80 Vcc;
  calcTdegC = (Toffset / 0.009) + 25.0;


  // conv. pin val to volts, calc instant. rotation rate, conv. to angle 
  // change in loop's interval, and add to current angle 
  gyroRate = (newGavgV / 1020) * gyroVolt; // conv. avg G pin AR (int) to volts (float)

  // calc the voltage offset from sitting still
  gyroRate -= gyroZeroVolt; // offset is prop. to instantaneous rate of rotation
  //  rest voltage varies, so we need occas. to reset w/ '0' command calling reZero fx

  // divide the offset by the gyro's sensitivity, to conv. to deg./sec, then by 
  // 100 since we have a 10ms loop delay (1000ms/10ms); must match loop delay
  gyroRate /= (gyroSens * 99);

  //Ignore the # if angular velocity does not exceed threshold
  if (fabs(gyroRate) >= rotatThres)   // abs val of float
  {
    currentAngle += gyroRate;  // add gyroRate to current Angle
  }

  //   //calc to keep angle between 0-359 degrees
  //   if (currentAngle < 0)
  //     currentAngle += 360;
  //   else if (currentAngle > 359)
  //     currentAngle -= 360;
  
    // read raw accel measurements from device
  accel.getAcceleration(&ax, &ay, &az); // I think:in C, passing by ref. means fx writes val back to var

  // print calc T,G values if different or > x sec have passed
  if (newTavgV != prevTavgV || newGavgV != prevGavgV || counter % 100 == 0)
  {     //print to Ser Mon if it's on

    //    Serial.print(Tvolt, 3);  // print w/ 3 digit precis. 
    //    Serial.println(" Tvolt");  //to calib. T you need to see this

    Serial.print(calcTdegC, 1);  // if calib good, you can calc real T
    Serial.println(" deg C");
    Serial.print("gyroZero "); 
    Serial.print(gyroZeroVolt,3);  // print w/ 3 digit precis.
    Serial.print("  gyrRate "); 
    Serial.println(gyroRate); // last value, change to add to currAngl
    Serial.print("Angle = "); 
    Serial.println(currentAngle);
    Serial.println();

    // print to LCD, if it's present
    lcd.clear();  // first clear screen and set the cursor to (0,0)       
    lcd.print("T= ");
    lcd.print(calcTdegC,1);  // temp in deg, 1 digit precis
    lcd.setCursor(0,1);   // second line
    lcd.print("ang "); 
    lcd.print(currentAngle,0);  // 0 precis, makes an int; ? if float is rounded or floor

    // display tab-separated accel x/y/z values
    Serial.print("accel:\t");
    Serial.print(ax); 
    Serial.print("\t");
    Serial.print(ay); 
    Serial.print("\t");
    Serial.println(az);
    Serial.println("**********************");
  } // end print if changed

  // blink LED to indicate activity every n loops
  if(counter % 100 == 0)  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
  counter++;  // augment by 1 each loop
  delay(10);  // 
}



