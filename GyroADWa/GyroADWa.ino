
/* integrate gyro ang. velocities over time intervals to get cumulative angle
 * Connect Gyro analog output to some Analog Pin, Temp output pin to another
 *
 * orig sketch by eric barch, ericbarch.com, got from Arduino playground/ Main/ interface w/ hardware
 * 2016-04-30 extensive mod by AM for Anal Device gyro ADW22307; make GyroADWb for experiments
 Uses: LCD display, anal gyro, stabiliz do/while to init,
*/

#define gyroPin A2          // gyro output to analog pin
#define tempPin A3         // temp may be relevant for drift calculation

//  include the library and init for 8x2 LCD
#include <LiquidCrystal.h>
// initialize new object with the Arduino pins it uses
LiquidCrystal lcd(12,11,4,5,6,7);

const float gyroVolt = 4.80;  //Gyro nominally 5V; put in measured val w/ AC or USB power and LCD on board
float gyroZeroVolt = 2.436;   //Gyro nominally outputs input/2 @ rest, but needs re-init. in setup and later
const float gyroSens = .007;  // ADW gyro nominally puts out 7mV/deg/sec; probably less sensitive than this
const float rotatThres = 0.01;   // minimum deg/.01 sec to recalc. angle - helps with gyro/Ardu noise
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

void setup() 
{
 
  // initialize serial communication at 9600 bps:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(8,2);
  
  delay(500);   // pause to read vals
  
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
}   // end setup


void loop() 
{
  static unsigned int counter = 0; // how many loops; static sets it once on first loop

  tempV = analogRead(tempPin);  // all these int AR values get conv. to float for MA calc
  newTavgV = (float)tempV*0.25+ 0.75*prevTavgV;  // updates expon moving average of Temp pin AR
  
  gyroV = analogRead(gyroPin);
  newGavgV = (float)gyroV*0.25 + 0.75*prevGavgV;  //updates expon moving average of G pin AR
                            // floats needed to avoid accumulating errors in calc.  but
                            // division very time consuming, ? use mult. instead

  //    check if full V input gives 1023 or what => both are 1019 w/ 4.78 or 4.99 Vcc supply
  //    : put 0 and full V input on the 2 AR pins, record voltage and AR; print 
  //  values if different or > 5 sec have passed; uncomment next block to calib like this
  
//  if (newTavgV != prevTavgV || newGavgV != prevGavgV || counter % 500 == 0)
//    {     //print to Ser Mon if it's on
//    Serial.print("T pin = "); 
//    Serial.print(newTavgV); 
//    Serial.print(" G pin = "); 
//    Serial.println(newGavgV);
//  
//    // print to LCD, if it's present
//    lcd.clear();  // first clear screen and set the cursor to (0,0)       
//    lcd.print("Tpin");
//    lcd.print(newTavgV);  // the averaged raw AR 
//    lcd.setCursor(0,1);   // second line
//    lcd.print("Gpin"); 
//    lcd.print(newGavgV);
//   } // end print if changed

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
  Tvolt = ((float)newTavgV / 1020) * gyroVolt;  // calc T voltage (float) from T pin AR; gVolt=Vcc
  // offset from voltage @ nominal 25 deg
  Toffset = Tvolt - 2.366; // 2.472 w/ 4.99 Vcc; was 2.361-6 w/ 4.80 Vcc;
  calcTdegC = (Toffset / 0.009) + 25.0;


  // conv. pin val to volts, calc instant. rotation rate, conv. to angle 
  // change in loop's interval, and add to current angle 
  gyroRate = (newGavgV / 1020) * gyroVolt; // conv. avg G pin AR (int) to volts (float)

  // calc the voltage offset from sitting still
  gyroRate -= gyroZeroVolt; // offset is prop. to instantaneous rate of rotation
  //  rest voltage varies, so we need occas. to reset w/ '0' command calling reZero() fx

  // divide the offset by the gyro's sensitivity, to conv. to deg./sec, then by 
  // ~100 since we have a 10ms loop delay (1000ms/10ms); important to match loop delay here
  gyroRate /= (gyroSens * 98);  // loop time sl. > 10 ms.

  // Ignore the # if angular velocity does not meet threshold
  if (fabs(gyroRate) >= rotatThres)   // abs val of float
  {
    currentAngle += gyroRate;  // add gyroRate to current Angle
  }

  //   //calc to keep angle between 0-359 degrees
  //   if (currentAngle < 0)
  //     currentAngle += 360;
  //   else if (currentAngle > 359)
  //     currentAngle -= 360;

  // print calc T,G values if different or > x sec have passed
  if (counter % 100 == 0)  // (newTavgV != prevTavgV || newGavgV != prevGavgV ||
  {     //print to Ser Mon if it's on

//    Serial.print(Tvolt, 3);  // print w/ 3 digit precis. 
//    Serial.println(" Tvolt");  //to calib. T you need to use this

    Serial.print(calcTdegC, 1);  // if calib good, you can calc real T
    Serial.println(" deg C");
//    Serial.print("gyroPinAR/V "); // for debug -- watch actual gyro AR and calc. voltage
//    Serial.print(newGavgV,1);Serial.print(" / "); Serial.println((newGavgV / 1020) * gyroVolt,3);
    Serial.print("gyroZero "); 
    Serial.print(gyroZeroVolt,3);  // print w/ 3 digit precis.
    Serial.print("  gyrRate "); 
    Serial.println(gyroRate,3); // last value, change to add to currAngl
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

  } // end print if changed
  
  prevTavgV = newTavgV;
  prevGavgV = newGavgV;   // updates for MA calc

  counter++;  // augment by 1 each loop
  delay(10);
}   // end loop


void reZero()  // resets 2 global vars: currentAngle to 0, gyroZeroVolt to its resting val 
{
  gyroV = analogRead(gyroPin);
  newGavgV = (float)gyroV*0.25 + 0.75*prevGavgV;  //updates expon moving average 
  gyroZeroVolt = (newGavgV / 1020) * gyroVolt;  // resting gyro voltage

  Serial.println("\nreZeroed angle and gyro");
  Serial.print("rest volts = "); 
  Serial.println(gyroZeroVolt,3); // 3 digit precis.
  Serial.println();

  lcd.clear();  // clear screen and set cursor to (0,0)       
  lcd.print("zeroing");
  lcd.setCursor(0,1);   // second line
  lcd.print("GV "); 
  lcd.print(gyroZeroVolt,3);  // 3 digit precis.
  currentAngle = 0;  //  zero out angle
  delay (2000);   
}

void reBase()  // resets gyroZeroVolt to its resting val, keeps currentAngle
{
  gyroV = analogRead(gyroPin);
  newGavgV = (float)gyroV*0.25 + 0.75*prevGavgV;  //updates expon moving average 
  gyroZeroVolt = (newGavgV / 1020) * gyroVolt;  // resting gyro voltage

  Serial.println("\nreZeroed gyro base V");
  Serial.print("rest volts = "); 
  Serial.println(gyroZeroVolt,3); // 3 digit precis.
  Serial.println();

  lcd.clear();  // clear screen and set cursor to (0,0)       
  lcd.print("zeroingV");
  lcd.setCursor(0,1);   // second line
  lcd.print("GV "); 
  lcd.print(gyroZeroVolt,3);  // 3 digit precis.
  delay (2000);   
}

/*  auto-reZeroing for base voltage instability
   // compare newGavgV to old avg AR, from N loops ago: if close to it & to gyroZero baseline
   // then you're stopped, so reset the gyroZeroV to current newGavgV -- auto-zeroing
  if(counter % 100 == 0)
    {
      if ( abs(oldGavgV - newGavgV)<=1 && abs(zeroPinV - newGavgV) <= 3 ) // 15 mv
         reBase();   // reset to new zero voltage
      else oldGavgV = newGavgV;      
    }
 */
